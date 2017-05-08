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

#include "connection.hpp"
#include "../../../backends/backend.hpp"
#include "../ofi.hpp"

pMR::OFI::Connection::Connection(Target const &target, Info info)
    : mFabric(info)
    , mDomain(mFabric, info)
    , mActiveEndpoint(mDomain, info)
    , mPassiveEndpoint(mDomain, info)
#ifdef OFI_RMA
    , mLocalTargetMemoryRegion(mDomain, mLocalTargetMemoryAddress.rawData(),
          {mLocalTargetMemoryAddress.size()}, FI_SEND)
    , mRemoteTargetMemoryRegion(mDomain, mRemoteTargetMemoryAddress.rawData(),
          {mRemoteTargetMemoryAddress.size()},
#ifdef OFI_RMA_CONTROL
          FI_REMOTE_WRITE)
#ifdef OFI_RMA_EVENT
    , mCounter(mDomain)
#endif // OFI_RMA_EVENT
#else
          FI_RECV)
#endif // OFI_RMA_CONTROL
#endif // OFI_RMA
{
    auto passiveAddress = mPassiveEndpoint.getAddress();
    auto activeAddress = mActiveEndpoint.getAddress();

    auto remotePassiveAddress = passiveAddress;
    auto remoteActiveAddress = activeAddress;

    Backend::exchange(target, passiveAddress, remotePassiveAddress);
    Backend::exchange(target, activeAddress, remoteActiveAddress);

    mPassiveEndpoint.connect(remoteActiveAddress);
    mActiveEndpoint.connect(remotePassiveAddress);

#ifdef OFI_RMA
#ifdef OFI_RMA_CONTROL
#ifdef OFI_RMA_TARGET_RX
    postRecvToActive();
#endif // OFI_RMA_TARGET_RX
#else
    postRecvAddressToActive();
#endif // OFI_RMA_CONTROL
#else
#ifndef OFI_NO_CONTROL
    postRecvToActive();
#endif // !OFI_NO_CONTROL
#endif // OFI_RMA

#ifdef OFI_RMA_CONTROL
    MemoryAddress localMemoryAddress(mRemoteTargetMemoryRegion);
    Backend::exchange(target, localMemoryAddress, mRemoteMemoryAddress);
#ifdef OFI_RMA_EVENT
    mRemoteTargetMemoryRegion.bind(mCounter);
#endif // OFI_RMA_EVENT
#else
    Backend::sync(target);
#endif // OFI_RMA_CONTROL
}

pMR::OFI::Domain &pMR::OFI::Connection::getDomain()
{
    return mDomain;
}

pMR::OFI::Domain const &pMR::OFI::Connection::getDomain() const
{
    return mDomain;
}

void pMR::OFI::Connection::checkMessageSize(std::size_t const size) const
{
    return mDomain.checkMessageSize({size});
}

void pMR::OFI::Connection::postSendToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    mActiveEndpoint.postSend(memoryRegion, {sizeByte});
}

void pMR::OFI::Connection::postSendToPassive()
{
    mPassiveEndpoint.postSend();
}

void pMR::OFI::Connection::postRecvToActive()
{
    mActiveEndpoint.postRecv();
}

void pMR::OFI::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    mPassiveEndpoint.postRecv(memoryRegion);
}

void pMR::OFI::Connection::postRecvToPassive()
{
    mPassiveEndpoint.postRecv();
}

void pMR::OFI::Connection::pollActiveSend()
{
    mActiveEndpoint.pollSend();
}

void pMR::OFI::Connection::pollActiveRecv()
{
#if defined OFI_RMA_CONTROL && defined OFI_RMA_EVENT
    mCounter.poll();
#else
    mActiveEndpoint.pollRecv();
#endif // OFI_RMA_CONTROL && OFI_RMA_EVENT
}

void pMR::OFI::Connection::pollPassiveSend()
{
    mPassiveEndpoint.pollSend();
}

void pMR::OFI::Connection::pollPassiveRecv()
{
    mPassiveEndpoint.pollRecv();
}

#ifdef OFI_RMA
void pMR::OFI::Connection::setLocalTargetMemoryAddress(
    MemoryRegion const &memoryRegion)
{
    mLocalTargetMemoryAddress.set(memoryRegion);
}

#ifdef OFI_RMA_CONTROL
void pMR::OFI::Connection::postWriteAddressToPassive()
{
    mPassiveEndpoint.postWrite(mLocalTargetMemoryRegion, mRemoteMemoryAddress);
}

#else
void pMR::OFI::Connection::postSendAddressToPassive()
{
    mPassiveEndpoint.postSend(mLocalTargetMemoryRegion);
}

void pMR::OFI::Connection::postRecvAddressToActive()
{
    mActiveEndpoint.postRecv(mRemoteTargetMemoryRegion);
}
#endif // OFI_RMA_CONTROL

void pMR::OFI::Connection::postWriteToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    mActiveEndpoint.postWrite(
        memoryRegion, mRemoteTargetMemoryAddress, {sizeByte});
}
#endif // OFI_RMA
