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
#include "../../../misc/singleton.hpp"
#include "../ofi.hpp"

pMR::OFI::Connection::Connection(Target const &target, Info info)
    : mEndpoint(&Singleton<GlobalEndpoint>::Instance(info))
    , mActiveEndpoint(mEndpoint)
    , mPassiveEndpoint(mEndpoint)
#ifdef OFI_RMA
    , mLocalTargetMemoryRegion(getDomain(), mLocalTargetMemoryAddress.rawData(),
          {mLocalTargetMemoryAddress.size()}, FI_SEND)
    , mRemoteTargetMemoryRegion(getDomain(),
          mRemoteTargetMemoryAddress.rawData(),
          {mRemoteTargetMemoryAddress.size()},
#ifdef OFI_RMA_CONTROL
          FI_REMOTE_WRITE)
#ifdef OFI_RMA_EVENT
    , mCounter(getDomain())
#endif // OFI_RMA_EVENT
#else
          FI_RECV)
#endif // OFI_RMA_CONTROL
#endif // OFI_RMA
{
    auto localAddress = mEndpoint->getAddress();
    auto activeID = mActiveEndpoint.getID();
    auto passiveID = mPassiveEndpoint.getID();

    auto peerAddress = localAddress;
    auto activeRemoteID = activeID;
    auto passiveRemoteID = passiveID;

    Backend::exchange(target, localAddress, peerAddress);
    Backend::exchange(target, activeID, passiveRemoteID);
    Backend::exchange(target, passiveID, activeRemoteID);

    mPeerAddress = {mEndpoint->addPeer(peerAddress)};
    mActiveEndpoint.setRemoteID(activeRemoteID);
    mPassiveEndpoint.setRemoteID(passiveRemoteID);

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
    return mEndpoint->getDomain();
}

pMR::OFI::Domain const &pMR::OFI::Connection::getDomain() const
{
    return mEndpoint->getDomain();
}

void pMR::OFI::Connection::checkMessageSize(std::size_t const size) const
{
    return mEndpoint->checkMessageSize({size});
}

void pMR::OFI::Connection::postSendToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    mActiveEndpoint.postSend(memoryRegion, {sizeByte}, {mPeerAddress});
}

void pMR::OFI::Connection::postSendToPassive()
{
    mPassiveEndpoint.postSend({mPeerAddress});
}

void pMR::OFI::Connection::postRecvToActive()
{
    mActiveEndpoint.postRecv({mPeerAddress});
}

void pMR::OFI::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    mPassiveEndpoint.postRecv(memoryRegion, {mPeerAddress});
}

void pMR::OFI::Connection::postRecvToPassive()
{
    mPassiveEndpoint.postRecv({mPeerAddress});
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
