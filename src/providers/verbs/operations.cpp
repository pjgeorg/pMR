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

#include "operations.hpp"
extern "C" {
#include <infiniband/verbs.h>
}
#include <stdexcept>
#include "verbs.hpp"

void pMR::verbs::postSendRequest(
    Endpoint &endpoint, ScatterGatherElement &scatterGatherElement)
{
    return postSendRequest(endpoint.getQueuePair(), scatterGatherElement);
}

void pMR::verbs::postSendRequest(
    QueuePair &queuePair, ScatterGatherElement &scatterGatherElement)
{
    ibv_send_wr workRequest = {};

    workRequest.wr_id = {VerbsSendWRID};
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = {scatterGatherElement.getNumEntries()};
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

void pMR::verbs::postRecvRequest(
    Endpoint &endpoint, ScatterGatherElement &scatterGatherElement)
{
    return postRecvRequest(endpoint.getQueuePair(), scatterGatherElement);
}

void pMR::verbs::postRecvRequest(
    QueuePair &queuePair, ScatterGatherElement &scatterGatherElement)
{
    ibv_recv_wr workRequest = {};

    workRequest.wr_id = {VerbsRecvWRID};
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = {scatterGatherElement.getNumEntries()};

    ibv_recv_wr *badRequest;

    if(ibv_post_recv(queuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Receive Work Request.");
    }
}

void pMR::verbs::postWriteRequest(Endpoint &endpoint,
    ScatterGatherElement &scatterGatherElement, MemoryAddress &target)
{
    return postWriteRequest(
        endpoint.getQueuePair(), scatterGatherElement, target);
}

void pMR::verbs::postWriteRequest(QueuePair &queuePair,
    ScatterGatherElement &scatterGatherElement, MemoryAddress &target)
{
    ibv_send_wr workRequest = {};

    workRequest.wr_id = {VerbsWriteWRID};
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = {scatterGatherElement.getNumEntries()};
    workRequest.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
    workRequest.wr.rdma.remote_addr = {target.getAddress()};
    workRequest.wr.rdma.rkey = {target.getRKey()};

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
