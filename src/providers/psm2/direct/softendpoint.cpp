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

#include "softendpoint.hpp"
#include "../../../misc/singleton.hpp"

pMR::psm2::SoftEndpoint::SoftEndpoint()
    : mEndpoint(&Singleton<GlobalEndpoint>::Instance())
{
}

psm2_epid_t pMR::psm2::SoftEndpoint::getID() const
{
    return mEndpoint->getID();
}

void pMR::psm2::SoftEndpoint::connect(psm2_epid_t const remoteEndpoint)
{
    mRemoteEndpoint = {mEndpoint->connect(remoteEndpoint)};
}

psm2_mq_req_t pMR::psm2::SoftEndpoint::postRecv(
    void *buffer, std::uint32_t sizeByte, std::uint64_t const rTag)
{
    return {mEndpoint->postRecv(mRemoteEndpoint, buffer, {sizeByte}, {rTag})};
}

psm2_mq_req_t pMR::psm2::SoftEndpoint::postSend(
    void const *buffer, std::uint32_t sizeByte, std::uint64_t const sTag)
{
    return {mEndpoint->postSend(mRemoteEndpoint, buffer, {sizeByte}, {sTag})};
}

void pMR::psm2::SoftEndpoint::poll(psm2_mq_req_t &request)
{
    mEndpoint->poll(request);
}
