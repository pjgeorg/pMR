#include "connection.hpp"
#include <stdexcept>
#include "verbs.hpp"
#include "../../backends/backend.hpp"

pMR::verbs::Connection::Connection(Target const &target,
        Device const &device, std::uint8_t const portNumber)
    :   mContext(device),
        mProtectionDomain(mContext),
        mSendCompletionQueue(mContext, VerbsMaxSend),
        mRecvCompletionQueue(mContext, VerbsMaxRecv),
        mQueuePair(mProtectionDomain, mSendCompletionQueue,
                mRecvCompletionQueue),
        mSendMemoryRegion(mContext, mProtectionDomain,
                mLocalMemoryAddress.rawData(), mLocalMemoryAddress.size(),
                IBV_ACCESS_LOCAL_WRITE),
        mRecvMemoryRegion(mContext, mProtectionDomain,
                mRemoteMemoryAddress.rawData(), mRemoteMemoryAddress.size(),
                IBV_ACCESS_LOCAL_WRITE)
{
    mQueuePair.setStateINIT(portNumber);
    ConnectionAddress originAddress =
        ConnectionAddress(mContext, mQueuePair, portNumber);
    ConnectionAddress targetAddress;
    exchangeConnectionAddress(target, originAddress, targetAddress);
    mQueuePair.setStateRTR(portNumber, targetAddress);

    for(int i=0; i!=VerbsIniRecv; ++i)
    {
        postRecvRequest();
    }

    // Sync with remote side to assure both sides are in state RTR.
    pMR::backend::sync(target);

    mQueuePair.setStateRTS();
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

void pMR::verbs::Connection::postRecvRequest()
{
    ScatterGatherList scatterGatherList(mRecvMemoryRegion);
    postRecvRequest(mQueuePair, scatterGatherList.get(), 1);
}

void pMR::verbs::Connection::postSendRequest()
{
    ScatterGatherList scatterGatherList(mSendMemoryRegion);
    postSendRequest(mQueuePair, scatterGatherList.get(), 1);
}

void pMR::verbs::Connection::postRDMAWriteRequest(
        MemoryRegion const &memoryRegion,
        MemoryAddress const &remoteMemoryAddress)
{
    ScatterGatherList scatterGatherList(memoryRegion);

    ibv_send_wr workRequest = {};
    workRequest.wr_id = VerbsRDMAWRID;
    workRequest.sg_list = scatterGatherList.get();
    workRequest.num_sge = 1;
    workRequest.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
    workRequest.send_flags = IBV_SEND_SIGNALED;
    workRequest.imm_data = 0;
    workRequest.wr.rdma.remote_addr = remoteMemoryAddress.getAddress();
    workRequest.wr.rdma.rkey = remoteMemoryAddress.getRKey();

    ibv_send_wr *badRequest;

    int ret;
    if((ret = ibv_post_send(mQueuePair.get(), &workRequest, &badRequest)))
    {
        throw std::runtime_error("pMR: Unable to post RDMA Work Request.");
    }
}

void pMR::verbs::Connection::pollSendCompletionQueue()
{
    mSendCompletionQueue.poll();
}

void pMR::verbs::Connection::pollRecvCompletionQueue()
{
    mRecvCompletionQueue.poll();
}

void pMR::verbs::Connection::initFence()
{
    postRecvRequest();
    postSendRequest();
}

void pMR::verbs::Connection::waitFence()
{
    pollSendCompletionQueue();
    pollRecvCompletionQueue();
}

void pMR::verbs::Connection::postRecvRequest(QueuePair &queuePair,
        ibv_sge *scatterGatherList, int length)
{
    ibv_recv_wr workRequest = {};
    workRequest.wr_id = VerbsRecvWRID;
    workRequest.sg_list = scatterGatherList;
    workRequest.num_sge = length;

    ibv_recv_wr *badRequest;

    if(ibv_post_recv(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Receive Work Request.");
    }
}

void pMR::verbs::Connection::postSendRequest(QueuePair &queuePair,
        ibv_sge *scatterGatherList, int const length)
{
    ibv_send_wr workRequest = {};
    workRequest.wr_id = VerbsSendWRID;
    workRequest.sg_list = scatterGatherList;
    workRequest.num_sge = length;
    workRequest.opcode = IBV_WR_SEND;
    workRequest.send_flags = IBV_SEND_SIGNALED;

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

ibv_sge* pMR::verbs::ScatterGatherList::get()
{
    return &mScatterGatherList;
}

ibv_sge const* pMR::verbs::ScatterGatherList::get() const
{
    return &mScatterGatherList;
}
