#include "queuepair.hpp"
#include <stdexcept>
#include "verbs.hpp"

pMR::verbs::QueuePair::QueuePair(ProtectionDomain &protectionDomain,
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
    initialAttributes.qp_type = IBV_QPT_RC;

    mQueuePair = ibv_create_qp(protectionDomain.get(), &initialAttributes);

    if(!mQueuePair)
    {
        throw std::runtime_error("pMR: Unable to create Queue Pair.");
    }
}

pMR::verbs::QueuePair::~QueuePair()
{
    ibv_destroy_qp(mQueuePair);
}

ibv_qp* pMR::verbs::QueuePair::get()
{
    return mQueuePair;
}

ibv_qp const* pMR::verbs::QueuePair::get() const
{
    return mQueuePair;
}

std::uint32_t pMR::verbs::QueuePair::getQPN() const
{
    return mQueuePair->qp_num;
}

void pMR::verbs::QueuePair::setStateINIT(std::uint8_t const portNumber)
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_INIT;
    attr.pkey_index = VerbsPKeyIndex;
    attr.port_num = portNumber;
    attr.qp_access_flags = IBV_ACCESS_REMOTE_WRITE;

    if(ibv_modify_qp(this->get(), &attr,
                IBV_QP_STATE |
                IBV_QP_PKEY_INDEX |
                IBV_QP_PORT |
                IBV_QP_ACCESS_FLAGS))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to INIT.");
    }
}

void pMR::verbs::QueuePair::setStateRTR(std::uint8_t const portNumber,
        ConnectionAddress const& targetAddress)
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_RTR;
    attr.path_mtu = targetAddress.getMTU();
    attr.dest_qp_num = targetAddress.getQPN();
    attr.rq_psn = VerbsPSN;
    attr.max_dest_rd_atomic = VerbsRDAtomic;
    attr.min_rnr_timer = VerbsRNRTimer;
    attr.ah_attr.dlid = targetAddress.getLID();
    attr.ah_attr.sl = VerbsServiceLevel;
    attr.ah_attr.src_path_bits = VerbsSrcPath;
    attr.ah_attr.port_num = portNumber;
    attr.ah_attr.grh.hop_limit = VerbsHopLimit;
    attr.ah_attr.grh.dgid = targetAddress.getGID();
    attr.ah_attr.grh.sgid_index = VerbsSGIDIndex;
    attr.ah_attr.is_global = VerbsGlobal;

    if(ibv_modify_qp(this->get(), &attr,
                IBV_QP_STATE |
                IBV_QP_PATH_MTU |
                IBV_QP_DEST_QPN |
                IBV_QP_RQ_PSN |
                IBV_QP_MAX_DEST_RD_ATOMIC |
                IBV_QP_MIN_RNR_TIMER |
                IBV_QP_AV ))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to RTR.");
    }
}

void pMR::verbs::QueuePair::setStateRTS()
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_RTS;
    attr.timeout = VerbsTimeout;
    attr.retry_cnt = VerbsRetry;
    attr.rnr_retry = VerbsRetry;
    attr.sq_psn = VerbsPSN;
    attr.max_rd_atomic = VerbsRDAtomic;

    if(ibv_modify_qp(this->get(), &attr,
                IBV_QP_STATE |
                IBV_QP_TIMEOUT |
                IBV_QP_RETRY_CNT |
                IBV_QP_RNR_RETRY |
                IBV_QP_SQ_PSN |
                IBV_QP_MAX_QP_RD_ATOMIC ))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to RTS.");
    }
}
