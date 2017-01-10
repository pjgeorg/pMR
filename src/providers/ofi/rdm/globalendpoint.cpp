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

pMR::ofi::GlobalEndpoint::GlobalEndpoint(Info &info)
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

    mSendCompletions.reserve(OFIReserveSizeCompletion);
    mRecvCompletions.reserve(OFIReserveSizeCompletion);
}

fid_ep *pMR::ofi::GlobalEndpoint::get()
{
    return mEndpoint.get();
}

fid_ep const *pMR::ofi::GlobalEndpoint::get() const
{
    return mEndpoint.get();
}

pMR::ofi::Domain &pMR::ofi::GlobalEndpoint::getDomain()
{
    return mDomain;
}

pMR::ofi::Domain const &pMR::ofi::GlobalEndpoint::getDomain() const
{
    return mDomain;
}

std::vector<std::uint8_t> pMR::ofi::GlobalEndpoint::getAddress() const
{
    return mEndpoint.getAddress();
}

fi_addr_t pMR::ofi::GlobalEndpoint::addPeer(
    std::vector<std::uint8_t> const &addr)
{
    return {mAddressVector.add(addr)};
}

void pMR::ofi::GlobalEndpoint::bind(
    std::uintptr_t const send, std::uintptr_t const recv)
{
    auto insert = mSendCompletions.insert(std::make_pair(send, 0));

    if(insert.second == false)
    {
        throw std::runtime_error("pMR: Unable to bind context.");
    }
    insert = mRecvCompletions.insert(std::make_pair(recv, 0));

    if(insert.second == false)
    {
        throw std::runtime_error("pMR: Unable to bind context.");
    }
}

void pMR::ofi::GlobalEndpoint::pollSend(std::uintptr_t const context)
{
    // First check if already retrieved completion event
    auto search = mSendCompletions.find(context);
    if(search == mSendCompletions.end())
    {
        throw std::logic_error("pMR: Polling unknown context.");
    }

    if(search->second > 0)
    {
        --search->second;
        return;
    }

    // Retrieve events from CQ
    while(true)
    {
        auto rContext = mSendCompletionQueue.poll();

        if(rContext == context)
        {
            return;
        }

        try
        {
            ++mSendCompletions.at(rContext);
        }
        catch(std::exception const &e)
        {
            throw std::runtime_error("pMR: Retrieved unknown Context.");
        }
    }
}

void pMR::ofi::GlobalEndpoint::pollRecv(std::uintptr_t const context)
{
    // First check if already retrieved completion event
    auto search = mRecvCompletions.find(context);
    if(search == mRecvCompletions.end())
    {
        throw std::logic_error("pMR: Polling unknown context.");
    }

    if(search->second > 0)
    {
        --search->second;
        return;
    }

    // Retrieve events from CQ
    while(true)
    {
        auto rContext = mRecvCompletionQueue.poll();

        if(rContext == context)
        {
            return;
        }

        try
        {
            ++mRecvCompletions.at(rContext);
        }
        catch(std::exception const &e)
        {
            throw std::runtime_error("pMR: Retrieved unknown Context.");
        }
    }
}

void pMR::ofi::GlobalEndpoint::unbind(
    std::uintptr_t const send, std::uintptr_t const recv)
{
    auto search = mSendCompletions.find(send);
    if(search == mSendCompletions.end())
    {
        throw std::runtime_error("pMR: Unable to unbind context.");
    }
    mSendCompletions.erase(search);

    search = mRecvCompletions.find(recv);
    if(search == mRecvCompletions.end())
    {
        throw std::runtime_error("pMR: Unable to unbind context.");
    }
    mRecvCompletions.erase(search);
}

void pMR::ofi::GlobalEndpoint::checkMessageSize(std::size_t const size) const
{
    return mDomain.checkMessageSize(size);
}

std::uint64_t pMR::ofi::GlobalEndpoint::checkInjectSize(std::size_t size) const
{
    return mDomain.checkInjectSize(size);
}
