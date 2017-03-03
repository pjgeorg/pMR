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

#include "globalendpoint.hpp"
#include <config.hpp>
#include "message.hpp"
#include "psm2.hpp"
#include "stats.hpp"

pMR::psm2::GlobalEndpoint::GlobalEndpoint() : mMatchedQueue(mEndpoint)
{
}

pMR::psm2::GlobalEndpoint::~GlobalEndpoint()
{
#ifdef PSM2_PRINT_STATS
    printStats(mMatchedQueue.get());
#endif // PSM2_PRINT_STATS
}

psm2_epid_t pMR::psm2::GlobalEndpoint::getID()
{
    if(ThreadLevel >= ThreadLevel::Multiple)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return {mEndpoint.getID()};
    }
    else
    {
        return {mEndpoint.getID()};
    }
}

psm2_epaddr_t pMR::psm2::GlobalEndpoint::connect(
    psm2_epid_t const remoteAddress)
{
    if(ThreadLevel >= ThreadLevel::Multiple)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return {mEndpoint.connect({remoteAddress})};
    }
    else
    {
        return {mEndpoint.connect({remoteAddress})};
    }
}

psm2_mq_req_t pMR::psm2::GlobalEndpoint::postRecv(psm2_epaddr_t const source,
    void *buffer, std::uint32_t sizeByte, std::uint64_t const rTag)
{
    if(ThreadLevel >= ThreadLevel::Multiple)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return {
            psm2::postRecv(mMatchedQueue, source, buffer, {sizeByte}, {rTag})};
    }
    else
    {
        return {
            psm2::postRecv(mMatchedQueue, source, buffer, {sizeByte}, {rTag})};
    }
}

psm2_mq_req_t pMR::psm2::GlobalEndpoint::postSend(
    psm2_epaddr_t const destination, void const *buffer, std::uint32_t sizeByte,
    std::uint64_t sTag)
{
    if(ThreadLevel >= ThreadLevel::Multiple)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return {psm2::postSend(
            mMatchedQueue, destination, buffer, {sizeByte}, {sTag})};
    }
    else
    {
        return {psm2::postSend(
            mMatchedQueue, destination, buffer, {sizeByte}, {sTag})};
    }
}

void pMR::psm2::GlobalEndpoint::poll(psm2_mq_req_t &request)
{
    if(ThreadLevel >= ThreadLevel::Multiple)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        psm2::poll(mEndpoint, request);
    }
    else
    {
        psm2::poll(mEndpoint, request);
    }
}
