//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "connection.hpp"
#include <stdexcept>
#include "verbs.hpp"
#include "../../backends/backend.hpp"

pMR::verbs::Connection::Connection(Target const &target,
        Device const &device, std::uint8_t const portNumber)
    :   mContext(device),
        mProtectionDomain(mContext),
#ifdef VERBS_RDMA
        mSendLocalAddress(mContext, mProtectionDomain,
                mLocalMemoryAddress.rawData(), mLocalMemoryAddress.size(),
                IBV_ACCESS_LOCAL_WRITE),
        mRecvRemoteAddress(mContext, mProtectionDomain,
                mRemoteMemoryAddress.rawData(), mRemoteMemoryAddress.size(),
                IBV_ACCESS_LOCAL_WRITE),
#endif // VERBS_RDMA
        mActiveCompletionQueue(mContext, VerbsMaxCQEntry),
        mPassiveCompletionQueue(mContext, VerbsMaxCQEntry),
        mActiveQueuePair(mProtectionDomain, mActiveCompletionQueue,
                mActiveCompletionQueue),
        mPassiveQueuePair(mProtectionDomain, mPassiveCompletionQueue,
                mPassiveCompletionQueue)
{
    mActiveQueuePair.setStateINIT(portNumber);
    mPassiveQueuePair.setStateINIT(portNumber);

    ConnectionAddress originActiveAddress =
        ConnectionAddress(mContext, mActiveQueuePair, portNumber);
    ConnectionAddress originPassiveAddress =
        ConnectionAddress(mContext, mPassiveQueuePair, portNumber);

    ConnectionAddress targetActiveAddress;
    ConnectionAddress targetPassiveAddress;

    exchangeConnectionAddress(target, originActiveAddress, originPassiveAddress,
            targetActiveAddress, targetPassiveAddress);

    mActiveQueuePair.setStateRTR(portNumber, targetPassiveAddress);
    mPassiveQueuePair.setStateRTR(portNumber, targetActiveAddress);

    for(int i = 0; i != VerbsInitialPostRecv; ++i)
    {
#ifdef VERBS_RDMA
        postRecvAddressToActive();
#else
        postRecvToActive();
#endif // VERBS_RDMA
    }

    pMR::backend::sync(target);

    mActiveQueuePair.setStateRTS();
    mPassiveQueuePair.setStateRTS();
}

pMR::verbs::Context& pMR::verbs::Connection::getContext()
{
    return mContext;
}

pMR::verbs::Context const& pMR::verbs::Connection::getContext() const
{
    return mContext;
}

pMR::verbs::ProtectionDomain& pMR::verbs::Connection::getProtectionDomain()
{
    return mProtectionDomain;
}

pMR::verbs::ProtectionDomain const&
pMR::verbs::Connection::getProtectionDomain() const
{
    return mProtectionDomain;
}

#ifdef VERBS_RDMA
void pMR::verbs::Connection::setLocalMemoryAddress(
        MemoryRegion const &memoryRegion)
{
    mLocalMemoryAddress.set(memoryRegion);
}

void pMR::verbs::Connection::postRecvAddressToActive()
{
    ScatterGatherElement scatterGatherElement(mRecvRemoteAddress);
    postRecvRequest(mActiveQueuePair, scatterGatherElement);
}

void pMR::verbs::Connection::postSendAddressToPassive()
{
    ScatterGatherElement scatterGatherElement(mSendLocalAddress);
    postSendRequest(mPassiveQueuePair, scatterGatherElement);
}

void pMR::verbs::Connection::postRecvToPassive()
{
    ScatterGatherElement scatterGatherElement;
    postRecvRequest(mPassiveQueuePair, scatterGatherElement);
}

void pMR::verbs::Connection::postWriteToActive(MemoryRegion const &memoryRegion,
        std::uint32_t const sizeByte)
{
    ScatterGatherElement scatterGatherElement(memoryRegion, sizeByte);
    postWriteRequest(mActiveQueuePair, scatterGatherElement);
}

#else
void pMR::verbs::Connection::postRecvToActive()
{
    ScatterGatherElement scatterGatherElement;
    postRecvRequest(mActiveQueuePair, scatterGatherElement);
}

void pMR::verbs::Connection::postSendToPassive()
{
    ScatterGatherElement scatterGatherElement;
    postSendRequest(mPassiveQueuePair, scatterGatherElement);
}

void pMR::verbs::Connection::postRecvToPassive(MemoryRegion const &memoryRegion)
{
    ScatterGatherElement scatterGatherElement(memoryRegion);
    postRecvRequest(mPassiveQueuePair, scatterGatherElement);
}

void pMR::verbs::Connection::postSendToActive(MemoryRegion const &memoryRegion,
        std::uint32_t const sizeByte)
{
    ScatterGatherElement scatterGatherElement(memoryRegion, sizeByte);
    postSendRequest(mActiveQueuePair, scatterGatherElement);
}
#endif // VERBS_RDMA

void pMR::verbs::Connection::pollActiveCompletionQueue()
{
    mActiveCompletionQueue.poll();
}

void pMR::verbs::Connection::pollPassiveCompletionQueue()
{
    mPassiveCompletionQueue.poll();
}

void pMR::verbs::Connection::postSendRequest(QueuePair &queuePair,
        ScatterGatherElement &scatterGatherElement)
{
    ibv_send_wr workRequest = {};

    workRequest.wr_id = VerbsSendWRID;
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = scatterGatherElement.getNumEntries();
    workRequest.opcode = IBV_WR_SEND;

    if(scatterGatherElement.getLength() <= VerbsMaxInlineDataSize)
    {
        workRequest.send_flags = IBV_SEND_INLINE;
    }

    ibv_send_wr *badRequest;

    if(ibv_post_send(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Send Work Request.");
    }
}

void pMR::verbs::Connection::postRecvRequest(QueuePair &queuePair,
        ScatterGatherElement &scatterGatherElement)
{
    ibv_recv_wr workRequest = {};

    workRequest.wr_id = VerbsRecvWRID;
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = scatterGatherElement.getNumEntries();

    ibv_recv_wr *badRequest;

    if(ibv_post_recv(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Receive Work Request.");
    }
}

#ifdef VERBS_RDMA
void pMR::verbs::Connection::postWriteRequest(QueuePair &queuePair,
        ScatterGatherElement &scatterGatherElement)
{
    ibv_send_wr workRequest = {};

    workRequest.wr_id = VerbsWriteWRID;
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = scatterGatherElement.getNumEntries();
    workRequest.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
    workRequest.wr.rdma.remote_addr = mRemoteMemoryAddress.getAddress();
    workRequest.wr.rdma.rkey = mRemoteMemoryAddress.getRKey();

    if(scatterGatherElement.getLength() <= VerbsMaxInlineDataSize)
    {
        workRequest.send_flags = IBV_SEND_INLINE;
    }

    ibv_send_wr *badRequest;

    if(ibv_post_send(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post RDMA Work Request.");
    }
}
#endif // VERBS_RDMA
