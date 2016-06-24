#include "connection.hpp"
#include <stdexcept>
#include "verbs.hpp"
#include "../../backends/backend.hpp"

pMR::verbs::Connection::Connection(Target const &target,
        Device const &device, std::uint8_t const portNumber)
    :   mContext(device),
        mProtectionDomain(mContext),
        mActiveCompletionQueue(mContext, VerbsMaxCQEntry),
        mPassiveCompletionQueue(mContext, VerbsMaxCQEntry),
        mActiveQueuePair(mProtectionDomain, mActiveCompletionQueue,
                mActiveCompletionQueue),
        mPassiveQueuePair(mProtectionDomain, mPassiveCompletionQueue,
                mPassiveCompletionQueue),
        mSendMemoryRegion(mContext, mProtectionDomain,
                mLocalMemoryAddress.rawData(), mLocalMemoryAddress.size(),
                IBV_ACCESS_LOCAL_WRITE),
        mRecvMemoryRegion(mContext, mProtectionDomain,
                mRemoteMemoryAddress.rawData(), mRemoteMemoryAddress.size(),
                IBV_ACCESS_LOCAL_WRITE)
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

    for(int i=0; i != VerbsInitialPostRecv; ++i)
    {
        postRecvAddrRequestToActive();
        postRecvSyncRequestToPassive();
    }

    // Sync with remote side to assure both sides are in state RTR.
    pMR::backend::sync(target);

    mActiveQueuePair.setStateRTS();
    mPassiveQueuePair.setStateRTS();

    // Get max inline data size
    mMaxInlineDataSize = mActiveQueuePair.getMaxInlineDataSize();
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

void pMR::verbs::Connection::setLocalMemoryAddress(
        MemoryRegion const &memoryRegion)
{
    mLocalMemoryAddress.set(memoryRegion);
}

pMR::verbs::MemoryAddress const&
pMR::verbs::Connection::getRemoteMemoryAddress() const
{
    return mRemoteMemoryAddress;
}

void pMR::verbs::Connection::postRecvSyncRequestToPassive()
{
    postRecvRequest(mPassiveQueuePair, nullptr, 0);
}

void pMR::verbs::Connection::postSendAddrRequestToPassive()
{
    ScatterGatherList scatterGatherList(mSendMemoryRegion);
    postSendRequest(mPassiveQueuePair, scatterGatherList.get(), 1);
}

void pMR::verbs::Connection::postRecvAddrRequestToActive()
{
    ScatterGatherList scatterGatherList(mRecvMemoryRegion);
    postRecvRequest(mActiveQueuePair, scatterGatherList.get(), 1);
}

void pMR::verbs::Connection::postSendSyncRequestToActive()
{
    postSendRequest(mActiveQueuePair, nullptr, 0);
}

void pMR::verbs::Connection::postRDMAWriteRequestToActive(
        MemoryRegion const &memoryRegion, std::uint32_t const sizeByte,
        MemoryAddress const &remoteMemoryAddress)
{
    ScatterGatherList scatterGatherList(memoryRegion, sizeByte);

    int sendFlags = IBV_SEND_SIGNALED;
    if(scatterGatherList.getLength() <= mMaxInlineDataSize)
    {
        sendFlags |= IBV_SEND_INLINE;
    }

    ibv_send_wr workRequest = {};
    workRequest.wr_id = VerbsRDMAWRID;
    workRequest.sg_list = scatterGatherList.get();
    workRequest.num_sge = scatterGatherList.getNumEntries();
    workRequest.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
    workRequest.send_flags = sendFlags;
    workRequest.imm_data = 0;
    workRequest.wr.rdma.remote_addr = remoteMemoryAddress.getAddress();
    workRequest.wr.rdma.rkey = remoteMemoryAddress.getRKey();

    ibv_send_wr *badRequest;

    int ret;
    if((ret = ibv_post_send(mActiveQueuePair.get(), &workRequest, &badRequest)))
    {
        throw std::runtime_error("pMR: Unable to post RDMA Work Request.");
    }
}

void pMR::verbs::Connection::pollActiveCompletionQueue()
{
    mActiveCompletionQueue.poll();
}

void pMR::verbs::Connection::pollPassiveCompletionQueue()
{
    mPassiveCompletionQueue.poll();
}

void pMR::verbs::Connection::postRecvRequest(QueuePair &queuePair,
        ibv_sge *scatterGatherList, int const numEntries)
{
    ibv_recv_wr workRequest = {};
    workRequest.wr_id = VerbsRecvWRID;
    workRequest.sg_list = scatterGatherList;
    workRequest.num_sge = numEntries;

    ibv_recv_wr *badRequest;

    if(ibv_post_recv(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Receive Work Request.");
    }
}

void pMR::verbs::Connection::postSendRequest(QueuePair &queuePair,
        ibv_sge *scatterGatherList, int const numEntries)
{
    int sendFlags = IBV_SEND_SIGNALED;
    if(scatterGatherList->length <= mMaxInlineDataSize)
    {
        sendFlags |= IBV_SEND_INLINE;
    }

    ibv_send_wr workRequest = {};
    workRequest.wr_id = VerbsSendWRID;
    workRequest.sg_list = scatterGatherList;
    workRequest.num_sge = numEntries;
    workRequest.opcode = IBV_WR_SEND;
    workRequest.send_flags = sendFlags;

    ibv_send_wr *badRequest;

    if(ibv_post_send(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Send Work Request.");
    }
}

pMR::verbs::ScatterGatherList::ScatterGatherList(
        MemoryRegion const& memoryRegion)
{
    mScatterGatherList = {};
    mScatterGatherList.addr = {memoryRegion.getAddress()};
    mScatterGatherList.length = {memoryRegion.getLength()};
    mScatterGatherList.lkey = {memoryRegion.getLKey()};
}

pMR::verbs::ScatterGatherList::ScatterGatherList(
        MemoryRegion const& memoryRegion, std::uint32_t const sizeByte)
{
    mScatterGatherList = {};
    mScatterGatherList.addr = {memoryRegion.getAddress()};
    mScatterGatherList.length = {sizeByte};
    mScatterGatherList.lkey = {memoryRegion.getLKey()};
}

ibv_sge* pMR::verbs::ScatterGatherList::get()
{
    return &mScatterGatherList;
}

ibv_sge const* pMR::verbs::ScatterGatherList::get() const
{
    return &mScatterGatherList;
}

std::uint32_t pMR::verbs::ScatterGatherList::getLength() const
{
    return mScatterGatherList.length;
}

int pMR::verbs::ScatterGatherList::getNumEntries() const
{
    if(mScatterGatherList.length == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
