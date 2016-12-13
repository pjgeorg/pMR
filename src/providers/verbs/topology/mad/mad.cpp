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

#include "mad.hpp"
#include <stdexcept>
#include "../../connection.hpp"
#include "../../scattergather.hpp"
#include "../../verbs.hpp"
#include "address.hpp"
#include "addresshandle.hpp"

pMR::verbs::mad::MAD::MAD(Context &context, std::uint8_t const portNumber)
    : mPortNumber{portNumber}
    , mPortAttributes(context, {mPortNumber})
    , mProtectionDomain(context)
    , mSendCompletionQueue(context, {VerbsMaxSend})
    , mRecvCompletionQueue(context, {VerbsMaxRecv})
    , mQueuePair(mProtectionDomain, mSendCompletionQueue, mRecvCompletionQueue)
    , mRecvMemoryRegion(context, mProtectionDomain,
          static_cast<void *>(mRecvMAD.data()),
          {static_cast<std::uint32_t>(sizeof(mRecvMAD))},
          IBV_ACCESS_LOCAL_WRITE)
{
    mQueuePair.setStateINIT(mPortNumber);
    mQueuePair.setStateRTR();
    mQueuePair.setStateRTS();
}

void pMR::verbs::mad::MAD::postRecvRequest()
{
    ScatterGatherElement scatterGatherElement(mRecvMemoryRegion);

    ibv_recv_wr workRequest = {};
    workRequest.wr_id = {VerbsRecvWRID};
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = 1;

    ibv_recv_wr *badRequest;

    if(ibv_post_recv(mQueuePair.get(), &workRequest, &badRequest))
    {
        throw std::runtime_error("pMR: Unable to post Receive Work Request.");
    }
}

void pMR::verbs::mad::MAD::postSendRequest()
{
    ScatterGatherElement scatterGatherElement(
        mSendMAD.data(), sizeof(mSendMAD));

    SubnetManager subnetManager(mPortAttributes, {mPortNumber});
    AddressHandle addressHandle(mProtectionDomain, subnetManager);

    ibv_send_wr workRequest = {};
    workRequest.wr_id = {VerbsSendWRID};
    workRequest.sg_list = scatterGatherElement.get();
    workRequest.num_sge = {scatterGatherElement.getNumEntries()};
    workRequest.opcode = IBV_WR_SEND;
    workRequest.send_flags = IBV_SEND_INLINE;
    workRequest.wr.ud.ah = addressHandle.get();
    workRequest.wr.ud.remote_qpn = 1;
    workRequest.wr.ud.remote_qkey = {VerbsDefaultQP1QKey};

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
    } while(!mRecvCompletionQueue.poll({VerbsMADPollCQRetry}));
}
