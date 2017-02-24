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
#include "../../backends/backend.hpp"
#include "../../misc/singleton.hpp"
#include "../ofi.hpp"

pMR::ofi::Connection::Connection(Target const &target, Info info)
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

    backend::exchange(target, localAddress, peerAddress);
    backend::exchange(target, activeID, passiveRemoteID);
    backend::exchange(target, passiveID, activeRemoteID);

    mPeerAddress = {mEndpoint->addPeer(peerAddress)};
    mActiveEndpoint.setRemoteID(activeRemoteID);
    mPassiveEndpoint.setRemoteID(passiveRemoteID);

#ifdef OFI_RMA
#ifdef OFI_RMA_CONTROL
#ifdef OFI_RMA_TARGET_RX
    mConnection->postRecvToActive();
#endif // OFI_RMA_TARGET_RX
#else
    mConnection->postRecvAddressToActive();
#endif // OFI_RMA_CONTROL
#else
#ifndef OFI_NO_CONTROL
    mConnection->postRecvToActive();
#endif // !OFI_NO_CONTROL
#endif // OFI_RMA

#ifdef OFI_RMA_CONTROL
    MemoryAddress localMemoryAddress(mRemoteTargetMemoryRegion);
    backend::exchange(target, localMemoryAddress, mRemoteMemoryAddress);
#ifdef OFI_RMA_EVENT
    mRemoteTargetMemoryRegion.bind(mCounter);
#endif // OFI_RMA_EVENT
#else
    backend::sync(target);
#endif // OFI_RMA_CONTROL
}

pMR::ofi::Domain &pMR::ofi::Connection::getDomain()
{
    return mEndpoint->getDomain();
}

pMR::ofi::Domain const &pMR::ofi::Connection::getDomain() const
{
    return mEndpoint->getDomain();
}

void pMR::ofi::Connection::checkMessageSize(std::size_t size) const
{
    return mEndpoint->checkMessageSize(size);
}

void pMR::ofi::Connection::postSendToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    postSend(mActiveEndpoint, memoryRegion, {sizeByte});
}

void pMR::ofi::Connection::postSendToPassive()
{
    postSend(mPassiveEndpoint);
}

void pMR::ofi::Connection::postRecvToActive()
{
    postRecv(mActiveEndpoint);
}

void pMR::ofi::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    postRecv(mPassiveEndpoint, memoryRegion);
}

void pMR::ofi::Connection::postRecvToPassive()
{
    postRecv(mPassiveEndpoint);
}

void pMR::ofi::Connection::pollActiveSend()
{
    mActiveEndpoint.pollSend();
}

void pMR::ofi::Connection::pollActiveRecv()
{
#if defined OFI_RMA_CONTROL && defined OFI_RMA_EVENT
    mCounter.poll();
#else
    mActiveEndpoint.pollRecv();
#endif // OFI_RMA_CONTROL && OFI_RMA_EVENT
}

void pMR::ofi::Connection::pollPassiveSend()
{
    mPassiveEndpoint.pollSend();
}

void pMR::ofi::Connection::pollPassiveRecv()
{
    mPassiveEndpoint.pollRecv();
}

#ifdef OFI_RMA
void pMR::ofi::Connection::setLocalTargetMemoryAddress(
    MemoryRegion const &memoryRegion)
{
    mLocalTargetMemoryAddress.set(memoryRegion);
}

#ifdef OFI_RMA_CONTROL
void pMR::ofi::Connection::postWriteAddressToPassive()
{
    postWrite(mPassiveEndpoint, mLocalTargetMemoryRegion, mRemoteMemoryAddress);
}

#else
void pMR::ofi::Connection::postSendAddressToPassive()
{
    postSend(mPassiveEndpoint, mLocalTargetMemoryRegion);
}

void pMR::ofi::Connection::postRecvAddressToActive()
{
    postRecv(mActiveEndpoint, mRemoteTargetMemoryRegion);
}
#endif // OFI_RMA_CONTROL

void pMR::ofi::Connection::postWriteToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    postWrite(
        mActiveEndpoint, memoryRegion, mRemoteTargetMemoryAddress, {sizeByte});
}
#endif // OFI_RMA

void pMR::ofi::Connection::postSend(
    SoftEndpoint &softEndpoint, MemoryRegion &memoryRegion)
{
    postSend(softEndpoint, memoryRegion, {memoryRegion.getLength()});
}

void pMR::ofi::Connection::postSend(SoftEndpoint &softEndpoint,
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    Tagged message(memoryRegion, {sizeByte}, softEndpoint.getSendContext(),
        {softEndpoint.getSendTag()}, {mPeerAddress});
    postSend(message);
}

void pMR::ofi::Connection::postSend(SoftEndpoint &softEndpoint)
{
    Tagged message(softEndpoint.getSendContext(), {softEndpoint.getSendTag()},
        {mPeerAddress});
    postSend(message);
}

void pMR::ofi::Connection::postSend(Tagged &message)
{
    postSendRequest(
        mEndpoint, message, mEndpoint->checkInjectSize({message.getLength()}));
}

void pMR::ofi::Connection::postRecv(
    SoftEndpoint &softEndpoint, MemoryRegion &memoryRegion)
{
    Tagged message(memoryRegion, softEndpoint.getRecvContext(),
        {softEndpoint.getRecvTag()}, {mPeerAddress});
    postRecv(message);
}

void pMR::ofi::Connection::postRecv(SoftEndpoint &softEndpoint)
{
    Tagged message(softEndpoint.getRecvContext(), {softEndpoint.getRecvTag()},
        {mPeerAddress});
    postRecv(message);
}

void pMR::ofi::Connection::postRecv(Tagged &message)
{
    postRecvRequest(mEndpoint, message);
}

void pMR::ofi::Connection::postWrite(SoftEndpoint &softEndpoint,
    MemoryRegion &memoryRegion, MemoryAddress &target)
{
    postWrite(softEndpoint, memoryRegion, target, {memoryRegion.getLength()});
}

void pMR::ofi::Connection::postWrite(SoftEndpoint &softEndpoint,
    MemoryRegion &memoryRegion, MemoryAddress &target,
    std::size_t const sizeByte)
{
    RMA message(memoryRegion, {sizeByte}, target, softEndpoint.getSendContext(),
        {softEndpoint.getRemoteID()}, {mPeerAddress});
    postWrite(message);
}

void pMR::ofi::Connection::postWrite(RMA &message)
{
    postWriteRequest(mEndpoint, message,
#ifndef OFI_RMA_EVENT
        FI_REMOTE_CQ_DATA |
#endif // OFI_RMA_EVENT
            mEndpoint->checkInjectSize({message.getLength()}));
}
