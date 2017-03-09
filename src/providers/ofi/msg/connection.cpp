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
#include <stdexcept>
#include "../../backends/backend.hpp"
#include "../common/passiveendpoint.hpp"
#include "../ofi.hpp"

pMR::OFI::Connection::Connection(Target const &target, Info info)
    : mFabric(info)
    , mDomain(mFabric, info)
#ifdef OFI_RMA
    , mLocalTargetMemoryRegion(mDomain, mLocalTargetMemoryAddress.rawData(),
          {mLocalTargetMemoryAddress.size()}, FI_SEND)
    , mRemoteTargetMemoryRegion(mDomain, mRemoteTargetMemoryAddress.rawData(),
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
    EventQueue eventQueue(mFabric);

    PassiveEndpoint passiveEndpoint(mFabric, info);
    passiveEndpoint.bind(eventQueue);
    passiveEndpoint.listen();

    std::vector<std::uint8_t> localAddress = passiveEndpoint.getAddress();
    std::vector<std::uint8_t> remoteAddress(localAddress.size());
    Backend::exchange(target, localAddress, remoteAddress);
    info.setDestinationAddress(remoteAddress);

    mActiveEndpoint = std::unique_ptr<SoftEndpoint>(
        new SoftEndpoint(mDomain, info, eventQueue));
    mActiveEndpoint->enable();

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
    Backend::exchange(target, localMemoryAddress, mRemoteMemoryAddress);
#ifdef OFI_RMA_EVENT
    mRemoteTargetMemoryRegion.bind(mCounter);
#endif // OFI_RMA_EVENT
#endif // OFI_RMA_CONTROL

    mActiveEndpoint->connect(remoteAddress);
    auto connReq = eventQueue.pollConnectionRequest();

    mPassiveEndpoint = std::unique_ptr<SoftEndpoint>(
        new SoftEndpoint(mDomain, connReq, eventQueue));
    mPassiveEndpoint->accept();

    eventQueue.pollConnected();
    eventQueue.pollConnected();
}

pMR::OFI::Domain &pMR::OFI::Connection::getDomain()
{
    return mDomain;
}

pMR::OFI::Domain const &pMR::OFI::Connection::getDomain() const
{
    return mDomain;
}

void pMR::OFI::Connection::checkMessageSize(std::size_t size) const
{
    return mDomain.checkMessageSize({size});
}

void pMR::OFI::Connection::postSendToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    postSend(mActiveEndpoint, memoryRegion, {sizeByte});
}

void pMR::OFI::Connection::postSendToPassive()
{
    postSend(mPassiveEndpoint);
}

void pMR::OFI::Connection::postRecvToActive()
{
    postRecv(mActiveEndpoint);
}

void pMR::OFI::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    postRecv(mPassiveEndpoint, memoryRegion);
}

void pMR::OFI::Connection::postRecvToPassive()
{
    postRecv(mPassiveEndpoint);
}

void pMR::OFI::Connection::pollActiveSend()
{
    mActiveEndpoint->pollSend();
}

void pMR::OFI::Connection::pollActiveRecv()
{
#if defined OFI_RMA_CONTROL && defined OFI_RMA_EVENT
    mCounter.poll();
#else
    mActiveEndpoint->pollRecv();
#endif // OFI_RMA_CONTROL && OFI_RMA_EVENT
}

void pMR::OFI::Connection::pollPassiveSend()
{
    mPassiveEndpoint->pollSend();
}

void pMR::OFI::Connection::pollPassiveRecv()
{
    mPassiveEndpoint->pollRecv();
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
    postWrite(mPassiveEndpoint, mLocalTargetMemoryRegion, mRemoteMemoryAddress);
}

#else
void pMR::OFI::Connection::postSendAddressToPassive()
{
    postSend(mPassiveEndpoint, mLocalTargetMemoryRegion);
}

void pMR::OFI::Connection::postRecvAddressToActive()
{
    postRecv(mActiveEndpoint, mRemoteTargetMemoryRegion);
}
#endif // OFI_RMA_CONTROL

void pMR::OFI::Connection::postWriteToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    postWrite(
        mActiveEndpoint, memoryRegion, mRemoteTargetMemoryAddress, {sizeByte});
}
#endif // OFI_RMA

void pMR::OFI::Connection::postSend(
    std::unique_ptr<SoftEndpoint> &softEndpoint, MemoryRegion &memoryRegion)
{
    postSend(softEndpoint, memoryRegion, {memoryRegion.getLength()});
}

void pMR::OFI::Connection::postSend(std::unique_ptr<SoftEndpoint> &softEndpoint,
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    Message message(memoryRegion, {sizeByte}, softEndpoint->getSendContext());
    postSend(softEndpoint, message);
}

void pMR::OFI::Connection::postSend(std::unique_ptr<SoftEndpoint> &softEndpoint)
{
    Message message(softEndpoint->getSendContext());
    postSend(softEndpoint, message);
}

void pMR::OFI::Connection::postSend(
    std::unique_ptr<SoftEndpoint> &softEndpoint, Message &message)
{
    postSendRequest(
        softEndpoint, message, mDomain.checkInjectSize({message.getLength()}));
}

void pMR::OFI::Connection::postRecv(
    std::unique_ptr<SoftEndpoint> &softEndpoint, MemoryRegion &memoryRegion)
{
    Message message(memoryRegion, softEndpoint->getRecvContext());
    postRecv(softEndpoint, message);
}

void pMR::OFI::Connection::postRecv(std::unique_ptr<SoftEndpoint> &softEndpoint)
{
    Message message(softEndpoint->getRecvContext());
    postRecv(softEndpoint, message);
}

void pMR::OFI::Connection::postRecv(
    std::unique_ptr<SoftEndpoint> &softEndpoint, Message &message)
{
    postRecvRequest(softEndpoint, message);
}

void pMR::OFI::Connection::postWrite(
    std::unique_ptr<SoftEndpoint> &softEndpoint, MemoryRegion &memoryRegion,
    MemoryAddress &target)
{
    postWrite(softEndpoint, memoryRegion, target, {memoryRegion.getLength()});
}

void pMR::OFI::Connection::postWrite(
    std::unique_ptr<SoftEndpoint> &softEndpoint, MemoryRegion &memoryRegion,
    MemoryAddress &target, std::size_t const sizeByte)
{
    RMA message(
        memoryRegion, {sizeByte}, target, softEndpoint->getSendContext());
    postWrite(softEndpoint, message);
}

void pMR::OFI::Connection::postWrite(
    std::unique_ptr<SoftEndpoint> &softEndpoint, RMA &message)
{
    postWriteRequest(softEndpoint, message,
#ifndef OFI_RMA_EVENT
        FI_REMOTE_CQ_DATA |
#endif // OFI_RMA_EVENT
            mDomain.checkInjectSize({message.getLength()}));
}
