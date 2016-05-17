#include "queuepair.hpp"
#include <stdexcept>
#include "../../verbs.hpp"
#include "../../queuepair.hpp"

pMR::verbs::mad::QueuePair::QueuePair(ProtectionDomain &protectionDomain,
        CompletionQueue &sendCompletionQueue,
        CompletionQueue &recvCompletionQueue)
{
    ibv_qp_init_attr initialAttributes = {};
    initialAttributes.send_cq = sendCompletionQueue.get();
    initialAttributes.recv_cq = recvCompletionQueue.get();
    initialAttributes.cap.max_send_wr = VerbsMaxSend;
    initialAttributes.cap.max_recv_wr = VerbsMaxRecv;
    initialAttributes.cap.max_send_sge = VerbsMaxSendSG;
    initialAttributes.cap.max_recv_sge = VerbsMaxRecvSG;
    initialAttributes.sq_sig_all = 1;
    initialAttributes.cap.max_inline_data = VerbsMADBlockSize;
    initialAttributes.qp_type = IBV_QPT_UD;

    mQueuePair = ibv_create_qp(protectionDomain.get(), &initialAttributes);

    if(!mQueuePair)
    {
        throw std::runtime_error("pMR: Unable to create Queue Pair.");
    }
}

pMR::verbs::mad::QueuePair::~QueuePair()
{
    ibv_destroy_qp(mQueuePair);
}

ibv_qp* pMR::verbs::mad::QueuePair::get()
{
    return mQueuePair;
}

ibv_qp const* pMR::verbs::mad::QueuePair::get() const
{
    return mQueuePair;
}

void pMR::verbs::mad::QueuePair::setStateINIT(std::uint8_t const portNumber)
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_INIT;
    attr.pkey_index = VerbsPKeyIndex;
    attr.port_num = portNumber;
    attr.qkey = VerbsDefaultQP1QKey;

    if(ibv_modify_qp(get(), &attr,
                IBV_QP_STATE |
                IBV_QP_PKEY_INDEX |
                IBV_QP_PORT |
                IBV_QP_QKEY))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to INIT.");
    }
}

void pMR::verbs::mad::QueuePair::setStateRTR()
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_RTR;

    if(ibv_modify_qp(get(), &attr, IBV_QP_STATE))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to RTR.");
    }
}

void pMR::verbs::mad::QueuePair::setStateRTS()
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_RTS;
    attr.sq_psn = VerbsPSN;

    if(ibv_modify_qp(get(), &attr, IBV_QP_STATE | IBV_QP_SQ_PSN))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to RTS.");
    }
}
