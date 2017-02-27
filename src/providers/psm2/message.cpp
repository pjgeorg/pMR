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

#include "message.hpp"
#include <stdexcept>
#include "error.hpp"
#include "psm2.hpp"
#include "matchedqueue.hpp"

psm2_mq_tag_t pMR::psm2::psmTag(std::uint64_t const tag)
{
    psm2_mq_tag_t psmTag;
    psmTag.tag0 = static_cast<std::uint32_t>(tag);
    psmTag.tag1 = static_cast<std::uint32_t>(tag >> 32);
    psmTag.tag2 = PSMExtraTag;

    return psmTag;
}

psm2_mq_req_t pMR::psm2::postRecv(MatchedQueue &mq, psm2_epaddr_t const source,
    void *buffer, std::uint32_t const sizeByte, std::uint64_t const rTag)
{
    auto tag = psmTag(rTag);
    auto tagSelector = PSMTagSelector;
    psm2_mq_req_t req;

    auto err = psm2_mq_irecv2(
        mq.get(), source, &tag, &tagSelector, 0, buffer, sizeByte, NULL, &req);

    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to post receive buffer: " + toString(err));
    }

    return req;
}

psm2_mq_req_t pMR::psm2::postSend(MatchedQueue &mq,
    psm2_epaddr_t const destination, void const *buffer,
    std::uint32_t const sizeByte, std::uint64_t const sTag)
{
    auto tag = psmTag(sTag);
    psm2_mq_req_t req;

    auto err = psm2_mq_isend2(
        mq.get(), destination, 0, &tag, buffer, sizeByte, NULL, &req);

    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to post send buffer: " + toString(err));
    }

    return req;
}

void pMR::psm2::poll(Endpoint &endpoint, psm2_mq_req_t &request)
{
    psm2_mq_status2_t status;
    do
    {
        psm2_poll(endpoint.get());
    } while(psm2_mq_test2(&request, &status) != PSM2_OK);

    if(status.error_code != PSM2_OK)
    {
        throw std::runtime_error(
            "pMR: Message status error: " + toString(status.error_code));
    }
}
