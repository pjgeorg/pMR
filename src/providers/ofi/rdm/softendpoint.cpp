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

pMR::OFI::SoftEndpoint::SoftEndpoint(Domain &domain, Info &info)
    : mEndpoint(domain, info)
    , mAddressVector(domain)
    , mCompletionQueue(domain, {info.getContextSize()})
    , mInjectSize{info.injectSize()}
{
    mEndpoint.bind(mAddressVector);
    mEndpoint.bind(mCompletionQueue, FI_TRANSMIT | FI_RECV);
    mEndpoint.enable();
}

std::vector<std::uint8_t> pMR::OFI::SoftEndpoint::getAddress() const
{
    return mEndpoint.getAddress();
}

void pMR::OFI::SoftEndpoint::connect(std::vector<std::uint8_t> const &addr)
{
    mPeerAddress = {mAddressVector.add(addr)};
}

fi_context *pMR::OFI::SoftEndpoint::getSendContext()
{
    return &mSendContext;
}

fi_context *pMR::OFI::SoftEndpoint::getRecvContext()
{
    return &mRecvContext;
}

void pMR::OFI::SoftEndpoint::postSend(
    MemoryRegion &memoryRegion)
{
    postSend(memoryRegion, {memoryRegion.getLength()});
}

void pMR::OFI::SoftEndpoint::postSend(MemoryRegion &memoryRegion,
    std::size_t const sizeByte)
{
    Message message(
        memoryRegion, {sizeByte}, getSendContext(), {mPeerAddress});
    postSend(message);
}

void pMR::OFI::SoftEndpoint::postSend()
{
    Message message(getSendContext(), {mPeerAddress});
    postSend(message);
}

void pMR::OFI::SoftEndpoint::postSend(Message &message)
{
    postSendRequest(mEndpoint, message, checkInjectSize({message.getLength()}));
}

void pMR::OFI::SoftEndpoint::postRecv(
    MemoryRegion &memoryRegion)
{
    Message message(memoryRegion, getRecvContext(), {mPeerAddress});
    postRecv(message);
}

void pMR::OFI::SoftEndpoint::postRecv()
{
    Message message(getRecvContext(), {mPeerAddress});
    postRecv(message);
}

void pMR::OFI::SoftEndpoint::postRecv(Message &message)
{
    postRecvRequest(mEndpoint, message);
}

void pMR::OFI::SoftEndpoint::postWrite(
    MemoryRegion &memoryRegion, MemoryAddress &target)
{
    postWrite(memoryRegion, target, {memoryRegion.getLength()});
}

void pMR::OFI::SoftEndpoint::postWrite(MemoryRegion &memoryRegion,
    MemoryAddress &target, std::size_t const sizeByte)
{
    RMA message(memoryRegion, {sizeByte}, target, getSendContext(),
        0, {mPeerAddress});
    postWrite(message);
}

void pMR::OFI::SoftEndpoint::postWrite(RMA &message)
{
    postWriteRequest(mEndpoint, message,
#ifndef OFI_RMA_EVENT
        FI_REMOTE_CQ_DATA |
#endif // OFI_RMA_EVENT
            checkInjectSize({message.getLength()}));
}

void pMR::OFI::SoftEndpoint::pollSend()
{
    mCompletionQueue.poll();
    return;
}

void pMR::OFI::SoftEndpoint::pollRecv()
{
    mCompletionQueue.poll();
    return;
}

std::uint64_t pMR::OFI::SoftEndpoint::checkInjectSize(std::size_t size) const
{
    return {OFI::checkInjectSize(size, mInjectSize)};
}
