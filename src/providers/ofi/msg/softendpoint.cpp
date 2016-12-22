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
#include <cstring>
#include <stdexcept>

pMR::ofi::SoftEndpoint::SoftEndpoint(
    Domain &domain, Info &info, EventQueue &eventQueue)
    : mEndpoint(domain, info), mCompletionQueue(domain, {info.getContextSize()})
{
    mEndpoint.bind(mCompletionQueue, FI_TRANSMIT | FI_RECV);
    mEndpoint.bind(eventQueue);
}

fid_ep *pMR::ofi::SoftEndpoint::get()
{
    return mEndpoint.get();
}

fid_ep const *pMR::ofi::SoftEndpoint::get() const
{
    return mEndpoint.get();
}

void pMR::ofi::SoftEndpoint::enable()
{
    return mEndpoint.enable();
}

void pMR::ofi::SoftEndpoint::connect(std::vector<std::uint8_t> const &address)
{
    return mEndpoint.connect(address);
}

void pMR::ofi::SoftEndpoint::accept()
{
    return mEndpoint.accept();
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
    mCompletionQueue.poll();
    return;
}

void pMR::ofi::SoftEndpoint::pollRecv()
{
    mCompletionQueue.poll();
    return;
}
