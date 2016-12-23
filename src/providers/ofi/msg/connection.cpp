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
#include "../common/message.hpp"
#include "../common/passiveendpoint.hpp"
#include "../common/rma.hpp"
#include "../ofi.hpp"

pMR::ofi::Connection::Connection(Target const &target, Info info)
    : mFabric(info)
    , mDomain(mFabric, info)
#ifdef OFI_RMA
    , mSendLocalAddress(mDomain, mLocalMemoryAddress.rawData(),
          {mLocalMemoryAddress.size()}, FI_SEND)
    , mRecvRemoteAddress(mDomain, mRemoteMemoryAddress.rawData(),
          {mRemoteMemoryAddress.size()}, FI_RECV)
#endif // OFI_RMA
{
    EventQueue eventQueue(mFabric);

    PassiveEndpoint passiveEndpoint(mFabric, info);
    passiveEndpoint.bind(eventQueue);
    passiveEndpoint.listen();

    std::vector<std::uint8_t> localAddress = passiveEndpoint.getAddress();
    std::vector<std::uint8_t> remoteAddress(localAddress.size());
    backend::exchange(target, localAddress, remoteAddress);

    info.setDestinationAddress(remoteAddress);

    mActiveEndpoint = std::unique_ptr<SoftEndpoint>(
        new SoftEndpoint(mDomain, info, eventQueue));
    mActiveEndpoint->enable();

#ifdef OFI_RMA
    postRecvAddressToActive();
#else
    postRecvToActive();
#endif // OFI_RMA

    mActiveEndpoint->connect(remoteAddress);
    auto connReq = eventQueue.pollConnectionRequest();

    mPassiveEndpoint = std::unique_ptr<SoftEndpoint>(
        new SoftEndpoint(mDomain, connReq, eventQueue));
    mPassiveEndpoint->accept();

    eventQueue.pollConnected();
    eventQueue.pollConnected();
}

pMR::ofi::Domain &pMR::ofi::Connection::getDomain()
{
    return mDomain;
}

pMR::ofi::Domain const &pMR::ofi::Connection::getDomain() const
{
    return mDomain;
}

void pMR::ofi::Connection::checkMessageSize(std::size_t size) const
{
    return mDomain.checkMessageSize({size});
}

#ifdef OFI_RMA
void pMR::ofi::Connection::setLocalMemoryAddress(
    MemoryRegion const &memoryRegion)
{
    mLocalMemoryAddress.set(memoryRegion);
}

void pMR::ofi::Connection::postRecvAddressToActive()
{
    Message message(mRecvRemoteAddress, mActiveEndpoint->getRecvContext());
    postRecvRequest(mActiveEndpoint, message);
}

void pMR::ofi::Connection::postSendAddressToPassive()
{
    Message message(mSendLocalAddress, mPassiveEndpoint->getSendContext());
    postSendRequest(mPassiveEndpoint, message,
        mDomain.checkInjectSize({message.getLength()}));
}

void pMR::ofi::Connection::postRecvToPassive()
{
    Message message(mPassiveEndpoint->getRecvContext());
    postRecvRequest(mPassiveEndpoint, message);
}

void pMR::ofi::Connection::postWriteToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    RMA message(memoryRegion, {sizeByte}, mRemoteMemoryAddress,
        mActiveEndpoint->getSendContext());
    postWriteRequest(mActiveEndpoint, message,
#ifndef OFI_RMA_EVENT
        FI_REMOTE_CQ_DATA |
#endif // OFI_RMA_EVENT
            mDomain.checkInjectSize({sizeByte}));
}

#else
void pMR::ofi::Connection::postRecvToActive()
{
    Message message(mActiveEndpoint->getRecvContext());
    postRecvRequest(mActiveEndpoint, message);
}

void pMR::ofi::Connection::postSendToPassive()
{
    Message message(mPassiveEndpoint->getSendContext());
    postSendRequest(mPassiveEndpoint, message,
        mDomain.checkInjectSize({message.getLength()}));
}

void pMR::ofi::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    Message message(memoryRegion, mPassiveEndpoint->getRecvContext());
    postRecvRequest(mPassiveEndpoint, message);
}

void pMR::ofi::Connection::postSendToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    Message message(
        memoryRegion, {sizeByte}, mActiveEndpoint->getSendContext());
    postSendRequest(mActiveEndpoint, message,
        mDomain.checkInjectSize({message.getLength()}));
}
#endif // OFI_RMA

void pMR::ofi::Connection::pollActiveSend()
{
    return mActiveEndpoint->pollSend();
}

void pMR::ofi::Connection::pollActiveRecv()
{
    return mActiveEndpoint->pollRecv();
}

void pMR::ofi::Connection::pollPassiveSend()
{
    return mPassiveEndpoint->pollSend();
}

void pMR::ofi::Connection::pollPassiveRecv()
{
    return mPassiveEndpoint->pollRecv();
}
