#include "mad.hpp"
#include <stdexcept>
#include "address.hpp"
#include "../../connection.hpp"

pMR::verbs::mad::MAD::MAD(Context &context,
        std::uint8_t const portNumber)
    :   mPortNumber(portNumber),
        mPortAttributes(context, mPortNumber),
        mProtectionDomain(context),
        mSendCompletionQueue(context, VerbsMaxSend),
        mRecvCompletionQueue(context, VerbsMaxRecv),
        mQueuePair(mProtectionDomain, mSendCompletionQueue,
                mRecvCompletionQueue),
        mRecvMemoryRegion(context, mProtectionDomain,
                static_cast<void*>(mRecvMAD.data()), sizeof(mRecvMAD),
                IBV_ACCESS_LOCAL_WRITE)
{
    mQueuePair.setStateINIT(mPortNumber);
    mQueuePair.setStateRTR();
    mQueuePair.setStateRTS();
}

void pMR::verbs::mad::MAD::postRecvRequest()
{
    ScatterGatherList scatterGatherList(mRecvMemoryRegion);

    ibv_recv_wr workRequest = {};
    workRequest.wr_id = VerbsRecvWRID;
    workRequest.sg_list = scatterGatherList.get();
    workRequest.num_sge = 1;

    ibv_recv_wr *badRequest;

    if(ibv_post_recv(mQueuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Receive Work Request.");
    }
}

void pMR::verbs::mad::MAD::postSendRequest()
{
    ibv_sge scatterGatherList = {};
    scatterGatherList.addr = reinterpret_cast<std::uint64_t>(mSendMAD.data());
    scatterGatherList.length = sizeof(mSendMAD);
    scatterGatherList.lkey = 0;

    SubnetManager subnetManager(mPortAttributes, mPortNumber);
    AddressHandle addressHandle(mProtectionDomain, subnetManager);

    ibv_send_wr workRequest = {};
    workRequest.wr_id = VerbsSendWRID;
    workRequest.sg_list = &scatterGatherList;
    workRequest.num_sge = 1;
    workRequest.opcode = IBV_WR_SEND;
    workRequest.send_flags = IBV_SEND_INLINE;
    workRequest.wr.ud.ah = addressHandle.get();
    workRequest.wr.ud.remote_qpn = 1;
    workRequest.wr.ud.remote_qkey = VerbsDefaultQP1QKey;

    ibv_send_wr *badRequest;

    if(ibv_post_send(mQueuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Send Work Request.");
    }
}

void pMR::verbs::mad::MAD::query()
{
    postRecvRequest();
    do
    {
        postSendRequest();
        mSendCompletionQueue.poll();
    }
    while(!mRecvCompletionQueue.poll(1e6));
}
