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

pMR::OFI::SoftEndpoint::SoftEndpoint(GlobalEndpoint *endpoint)
    : mEndpoint(endpoint)
{
    mEndpoint->bind({reinterpret_cast<std::uintptr_t>(&mSendContext)},
        {reinterpret_cast<std::uintptr_t>(&mRecvContext)});
}

pMR::OFI::SoftEndpoint::~SoftEndpoint()
{
    mEndpoint->unbind({reinterpret_cast<std::uintptr_t>(&mSendContext)},
        {reinterpret_cast<std::uintptr_t>(&mRecvContext)});
}

std::uint64_t pMR::OFI::SoftEndpoint::getID() const
{
    return {reinterpret_cast<std::uintptr_t>(&mRecvContext)};
}

void pMR::OFI::SoftEndpoint::setRemoteID(std::uint64_t remoteID)
{
    mRemoteID = {remoteID};
}

std::uint64_t pMR::OFI::SoftEndpoint::getRemoteID() const
{
    return {mRemoteID};
}

std::uint64_t pMR::OFI::SoftEndpoint::getSendTag() const
{
    return {getRemoteID()};
}

std::uint64_t pMR::OFI::SoftEndpoint::getRecvTag() const
{
    return {getID()};
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
    MemoryRegion &memoryRegion, fi_addr_t const address)
{
    postSend(memoryRegion, {memoryRegion.getLength()}, {address});
}

void pMR::OFI::SoftEndpoint::postSend(MemoryRegion &memoryRegion,
    std::size_t const sizeByte, fi_addr_t const address)
{
    Tagged message(
        memoryRegion, {sizeByte}, getSendContext(), {getSendTag()}, {address});
    postSend(message);
}

void pMR::OFI::SoftEndpoint::postSend(fi_addr_t const address)
{
    Tagged message(getSendContext(), {getSendTag()}, {address});
    postSend(message);
}

void pMR::OFI::SoftEndpoint::postSend(Tagged &message)
{
    mEndpoint->postSend(message);
}

void pMR::OFI::SoftEndpoint::postRecv(
    MemoryRegion &memoryRegion, fi_addr_t const address)
{
    Tagged message(memoryRegion, getRecvContext(), {getRecvTag()}, {address});
    postRecv(message);
}

void pMR::OFI::SoftEndpoint::postRecv(fi_addr_t const address)
{
    Tagged message(getRecvContext(), {getRecvTag()}, {address});
    postRecv(message);
}

void pMR::OFI::SoftEndpoint::postRecv(Tagged &message)
{
    mEndpoint->postRecv(message);
}

void pMR::OFI::SoftEndpoint::postWrite(
    MemoryRegion &memoryRegion, MemoryAddress &target, fi_addr_t const address)
{
    postWrite(memoryRegion, target, {memoryRegion.getLength()}, {address});
}

void pMR::OFI::SoftEndpoint::postWrite(MemoryRegion &memoryRegion,
    MemoryAddress &target, std::size_t const sizeByte, fi_addr_t const address)
{
    RMA message(memoryRegion, {sizeByte}, target, getSendContext(),
        {getRemoteID()}, {address});
    postWrite(message);
}

void pMR::OFI::SoftEndpoint::postWrite(RMA &message)
{
    mEndpoint->postWrite(message);
}

void pMR::OFI::SoftEndpoint::pollSend()
{
    mEndpoint->pollSend({reinterpret_cast<std::uintptr_t>(&mSendContext)});
}

void pMR::OFI::SoftEndpoint::pollRecv()
{
    mEndpoint->pollRecv({reinterpret_cast<std::uintptr_t>(&mRecvContext)});
}
