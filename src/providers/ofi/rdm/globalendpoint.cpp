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
#include <stdexcept>
#include "../ofi.hpp"

pMR::OFI::GlobalEndpoint::GlobalEndpoint(Info &info)
    : mFabric(info)
    , mDomain(mFabric, info)
    , mEndpoint(mDomain, info)
    , mAddressVector(mDomain)
    , mSendCompletionQueue(mDomain, {info.getContextSize()})
    , mRecvCompletionQueue(mDomain, {info.getContextSize()})
    , mMaxSize{info.maxSize()}
    , mInjectSize{info.injectSize()}
{
    mEndpoint.bind(mAddressVector);
    mEndpoint.bind(mSendCompletionQueue, FI_TRANSMIT);
    mEndpoint.bind(mRecvCompletionQueue, FI_RECV);
    mEndpoint.enable();

    mSendCompletions.reserve(cReserveSizeCompletion);
    mRecvCompletions.reserve(cReserveSizeCompletion);
}

fid_ep *pMR::OFI::GlobalEndpoint::get()
{
    return mEndpoint.get();
}

fid_ep const *pMR::OFI::GlobalEndpoint::get() const
{
    return mEndpoint.get();
}

pMR::OFI::Domain &pMR::OFI::GlobalEndpoint::getDomain()
{
    return mDomain;
}

pMR::OFI::Domain const &pMR::OFI::GlobalEndpoint::getDomain() const
{
    return mDomain;
}

std::vector<std::uint8_t> pMR::OFI::GlobalEndpoint::getAddress() const
{
    return mEndpoint.getAddress();
}

fi_addr_t pMR::OFI::GlobalEndpoint::addPeer(
    std::vector<std::uint8_t> const &addr)
{
    return {mAddressVector.add(addr)};
}

void pMR::OFI::GlobalEndpoint::bind(
    std::uint64_t const sendID, std::uint64_t const recvID)
{
    if(cThreadLevel >= ThreadLevel::Multiple)
    {
        {
            std::lock_guard<std::mutex> lock(mSendCompletionsMutex);
            bind(mSendCompletions, sendID);
        }
        {
            std::lock_guard<std::mutex> lock(mRecvCompletionsMutex);
            bind(mRecvCompletions, recvID);
        }
    }
    else
    {
        bind(mSendCompletions, sendID);
        bind(mRecvCompletions, recvID);
    }
}

void pMR::OFI::GlobalEndpoint::unbind(
    std::uint64_t const sendID, std::uint64_t const recvID)
{
    if(cThreadLevel >= ThreadLevel::Multiple)
    {
        {
            std::lock_guard<std::mutex> lock(mSendCompletionsMutex);
            unbind(mSendCompletions, sendID);
        }
        {
            std::lock_guard<std::mutex> lock(mRecvCompletionsMutex);
            unbind(mRecvCompletions, recvID);
        }
    }
    else
    {
        unbind(mSendCompletions, sendID);
        unbind(mRecvCompletions, recvID);
    }
}

void pMR::OFI::GlobalEndpoint::pollSend(std::uint64_t const iD)
{
    poll(mSendCompletionQueue, mSendCompletionQueueMutex, mSendCompletions,
        mSendCompletionsMutex, iD);
}

void pMR::OFI::GlobalEndpoint::pollRecv(std::uint64_t const iD)
{
    poll(mRecvCompletionQueue, mRecvCompletionQueueMutex, mRecvCompletions,
        mRecvCompletionsMutex, iD);
}

void pMR::OFI::GlobalEndpoint::checkMessageSize(std::size_t const size) const
{
    return mDomain.checkMessageSize(size);
}

std::uint64_t pMR::OFI::GlobalEndpoint::checkInjectSize(std::size_t size) const
{
    return mDomain.checkInjectSize(size);
}

void pMR::OFI::GlobalEndpoint::bind(
    std::unordered_map<std::uint64_t, int> &map, std::uint64_t const iD)
{
    auto insert = map.insert(std::make_pair(iD, 0));

    if(insert.second == false)
    {
        throw std::runtime_error("pMR: Unable to bind ID.");
    }
}

void pMR::OFI::GlobalEndpoint::unbind(
    std::unordered_map<std::uint64_t, int> &map, std::uint64_t const iD)
{
    auto search = map.find(iD);

    if(search == map.end())
    {
        throw std::runtime_error("pMR: Unable to unbind ID.");
    }

    map.erase(search);
}

bool pMR::OFI::GlobalEndpoint::checkCompletions(
    std::unordered_map<std::uint64_t, int> &map, std::uint64_t const iD)
{
    auto search = map.find(iD);
    if(search == map.end())
    {
        throw std::logic_error("pMR: Polling unknown ID.");
    }

    if(search->second > 0)
    {
        --search->second;
        return {true};
    }
    else
    {
        return {false};
    }
}

std::uint64_t pMR::OFI::GlobalEndpoint::retrieveCompletions(
    CompletionQueueContext &queue)
{
    return {queue.poll()};
}

std::uint64_t pMR::OFI::GlobalEndpoint::retrieveCompletions(
    CompletionQueueData &queue)
{
    auto entry = queue.poll();

    if(entry.second != 0)
    {
        return {entry.second};
    }
    else
    {
        return {entry.first};
    }
}
