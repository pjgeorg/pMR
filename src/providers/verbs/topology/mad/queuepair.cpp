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

#include "queuepair.hpp"
#include <stdexcept>
#include "../../queuepair.hpp"
#include "../../verbs.hpp"

pMR::Verbs::MAD::QueuePair::QueuePair(ProtectionDomain &protectionDomain,
    CompletionQueue &sendCompletionQueue, CompletionQueue &recvCompletionQueue)
{
    ibv_qp_init_attr initialAttributes = {};
    initialAttributes.send_cq = sendCompletionQueue.get();
    initialAttributes.recv_cq = recvCompletionQueue.get();
    initialAttributes.cap.max_send_wr = {cMaxSend};
    initialAttributes.cap.max_recv_wr = {cMaxRecv};
    initialAttributes.cap.max_send_sge = {cMaxSendSG};
    initialAttributes.cap.max_recv_sge = {cMaxRecvSG};
    initialAttributes.sq_sig_all = 1;
    initialAttributes.cap.max_inline_data = {cMADBlockSize};
    initialAttributes.qp_type = IBV_QPT_UD;

    mQueuePair = ibv_create_qp(protectionDomain.get(), &initialAttributes);

    if(!mQueuePair)
    {
        throw std::runtime_error("pMR: Unable to create Queue Pair.");
    }
}

pMR::Verbs::MAD::QueuePair::~QueuePair()
{
    ibv_destroy_qp(mQueuePair);
}

ibv_qp *pMR::Verbs::MAD::QueuePair::get()
{
    return mQueuePair;
}

ibv_qp const *pMR::Verbs::MAD::QueuePair::get() const
{
    return mQueuePair;
}

void pMR::Verbs::MAD::QueuePair::setStateINIT(std::uint8_t const portNumber)
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_INIT;
    attr.pkey_index = {cPKeyIndex};
    attr.port_num = {portNumber};
    attr.qkey = {cDefaultQP1QKey};

    if(ibv_modify_qp(get(), &attr,
           IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_QKEY))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to INIT.");
    }
}

void pMR::Verbs::MAD::QueuePair::setStateRTR()
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_RTR;

    if(ibv_modify_qp(get(), &attr, IBV_QP_STATE))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to RTR.");
    }
}

void pMR::Verbs::MAD::QueuePair::setStateRTS()
{
    ibv_qp_attr attr = {};
    attr.qp_state = IBV_QPS_RTS;
    attr.sq_psn = {cPSN};

    if(ibv_modify_qp(get(), &attr, IBV_QP_STATE | IBV_QP_SQ_PSN))
    {
        throw std::runtime_error("pMR: Unable to modify QueuePair to RTS.");
    }
}
