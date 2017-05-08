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

#include "domain.hpp"
#include <stdexcept>
extern "C" {
#include <rdma/fi_domain.h>
}

pMR::OFI::Domain::Domain(Fabric &fabric, Info &info)
    : mMaxSize{info.maxSize()}, mInjectSize{info.injectSize()}
{
    if(fi_domain(fabric.get(), info.get(), &mDomain, &mContext))
    {
        throw std::runtime_error("pMR: Unable to create domain.");
    }
}

pMR::OFI::Domain::~Domain()
{
    if(mDomain)
    {
        fi_close(&mDomain->fid);
    }
}

fid_domain *pMR::OFI::Domain::get()
{
    return mDomain;
}

fid_domain const *pMR::OFI::Domain::get() const
{
    return mDomain;
}

void pMR::OFI::Domain::checkMessageSize(std::size_t const size) const
{
    if(size > mMaxSize)
    {
        throw std::length_error("pMR: Message size overflow.");
    }
}

std::uint64_t pMR::OFI::Domain::checkInjectSize(std::size_t const size) const
{
    return {OFI::checkInjectSize(size, mInjectSize)};
}

std::uint64_t pMR::OFI::checkInjectSize(
    std::size_t const size, std::size_t const maxInjectSize)
{
    if(size <= maxInjectSize)
    {
        return {FI_INJECT};
    }
    else
    {
        return 0;
    }
}
