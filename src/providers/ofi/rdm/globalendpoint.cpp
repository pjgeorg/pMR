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
    , mCompletionQueue(mDomain, {info.getContextSize()})
    , mMaxSize{info.maxSize()}
    , mInjectSize{info.injectSize()}
{
    mEndpoint.bind(mAddressVector);
    mEndpoint.bind(mCompletionQueue, FI_TRANSMIT | FI_RECV);
    mEndpoint.enable();

    mCompletions.reserve(OFIReserveSizeCompletion);
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

void pMR::ofi::GlobalEndpoint::bind(std::uintptr_t const &context)
{
    auto insert = mCompletions.insert(std::make_pair(context, 0));

    if(insert.second == false)
    {
        throw std::runtime_error("pMR: Unable to bind context.");
    }
}

void pMR::ofi::GlobalEndpoint::poll(std::uintptr_t const &context)
{
    // First check if already retrieved completion event
    auto search = mCompletions.find(context);
    if(search == mCompletions.end())
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
        auto rContext = mCompletionQueue.poll();

        if(rContext == context)
        {
            return;
        }

        try
        {
            ++mCompletions.at(rContext);
        }
        catch(std::exception const &e)
        {
            throw std::runtime_error("pMR: Retrieved unknown Context.");
        }
    }
}

void pMR::ofi::GlobalEndpoint::unbind(std::uintptr_t const &context)
{
    auto search = mCompletions.find(context);
    if(search == mCompletions.end())
    {
        throw std::runtime_error("pMR: Unable to unbind context.");
    }

    mCompletions.erase(search);
}

void pMR::ofi::GlobalEndpoint::checkMessageSize(std::size_t const size) const
{
    return mDomain.checkMessageSize(size);
}

std::uint64_t pMR::ofi::GlobalEndpoint::checkInjectSize(std::size_t size) const
{
    return mDomain.checkInjectSize(size);
}
