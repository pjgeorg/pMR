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

pMR::ofi::SoftEndpoint::SoftEndpoint(GlobalEndpoint *endpoint)
    : mEndpoint(endpoint)
{
    mEndpoint->bind({reinterpret_cast<std::uintptr_t>(&mSendContext)},
        {reinterpret_cast<std::uintptr_t>(&mRecvContext)});
}

pMR::ofi::SoftEndpoint::~SoftEndpoint()
{
    mEndpoint->unbind({reinterpret_cast<std::uintptr_t>(&mSendContext)},
        {reinterpret_cast<std::uintptr_t>(&mRecvContext)});
}

std::uint64_t pMR::ofi::SoftEndpoint::getID() const
{
    return {reinterpret_cast<std::uintptr_t>(&mRecvContext)};
}

void pMR::ofi::SoftEndpoint::setRemoteID(std::uint64_t remoteID)
{
    mRemoteID = {remoteID};
}

std::uint64_t pMR::ofi::SoftEndpoint::getRemoteID() const
{
    return {mRemoteID};
}

std::uint64_t pMR::ofi::SoftEndpoint::getSendTag() const
{
    return {getRemoteID()};
}

std::uint64_t pMR::ofi::SoftEndpoint::getRecvTag() const
{
    return {getID()};
}

fi_context *pMR::ofi::SoftEndpoint::getSendContext()
{
    return &mSendContext;
}

fi_context *pMR::ofi::SoftEndpoint::getRecvContext()
{
    return &mRecvContext;
}

void pMR::ofi::SoftEndpoint::pollSend()
{
    mEndpoint->pollSend({reinterpret_cast<std::uintptr_t>(&mSendContext)});
}

void pMR::ofi::SoftEndpoint::pollRecv()
{
    mEndpoint->pollRecv({reinterpret_cast<std::uintptr_t>(&mRecvContext)});
}
