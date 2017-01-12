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
#include "../common/rma.hpp"
#include "../common/tagged.hpp"
#include "../ofi.hpp"

pMR::ofi::Connection::Connection(Target const &target, Info info)
    : mEndpoint(&Singleton<GlobalEndpoint>::Instance(info))
    , mActiveEndpoint(mEndpoint)
    , mPassiveEndpoint(mEndpoint)
#ifdef OFI_RMA
    , mSendLocalAddress(getDomain(), mLocalMemoryAddress.rawData(),
          {mLocalMemoryAddress.size()}, FI_SEND)
    , mRecvRemoteAddress(getDomain(), mRemoteMemoryAddress.rawData(),
          {mRemoteMemoryAddress.size()}, FI_RECV)
#endif // OFI_RMA
{
    auto localAddress = mEndpoint->getAddress();
    auto activeSendTag = mActiveEndpoint.getSendTag();
    auto passiveSendTag = mPassiveEndpoint.getSendTag();

    auto peerAddress = localAddress;
    auto activeRecvTag = activeSendTag;
    auto passiveRecvTag = passiveSendTag;

    backend::exchange(target, localAddress, peerAddress);
    backend::exchange(target, activeSendTag, passiveRecvTag);
    backend::exchange(target, passiveSendTag, activeRecvTag);

    mPeerAddress = {mEndpoint->addPeer(peerAddress)};
    mActiveEndpoint.setRecvTag(activeRecvTag);
    mPassiveEndpoint.setRecvTag(passiveRecvTag);

#ifdef OFI_RMA
    postRecvAddressToActive();
#else
    postRecvToActive();
#endif // OFI_RMA

    backend::sync(target);
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

#ifdef OFI_RMA
void pMR::ofi::Connection::setLocalMemoryAddress(
    MemoryRegion const &memoryRegion)
{
    mLocalMemoryAddress.set(memoryRegion);
}

void pMR::ofi::Connection::postRecvAddressToActive()
{
    Tagged message(mRecvRemoteAddress, mActiveEndpoint.getRecvContext(),
        {mActiveEndpoint.getRecvTag()}, {mPeerAddress});
    postRecvRequest(mEndpoint, message);
}

void pMR::ofi::Connection::postSendAddressToPassive()
{
    Tagged message(mSendLocalAddress, mPassiveEndpoint.getSendContext(),
        {mPassiveEndpoint.getSendTag()}, {mPeerAddress});
    postSendRequest(
        mEndpoint, message, mEndpoint->checkInjectSize({message.getLength()}));
}

void pMR::ofi::Connection::postRecvToPassive()
{
    Tagged message(mPassiveEndpoint.getRecvContext(),
        {mPassiveEndpoint.getRecvTag()}, {mPeerAddress});
    postRecvRequest(mEndpoint, message);
}

void pMR::ofi::Connection::postWriteToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    RMA message(memoryRegion, {sizeByte}, mRemoteMemoryAddress,
        mActiveEndpoint.getSendContext(), {mActiveEndpoint.getRemoteID()},
        {mPeerAddress});
    postWriteRequest(mEndpoint, message,
#ifndef OFI_RMA_EVENT
        FI_REMOTE_CQ_DATA |
#endif // OFI_RMA_EVENT
            mEndpoint->checkInjectSize({sizeByte}));
}

#else
void pMR::ofi::Connection::postRecvToActive()
{
    Tagged message(mActiveEndpoint.getRecvContext(),
        {mActiveEndpoint.getRecvTag()}, {mPeerAddress});
    postRecvRequest(mEndpoint, message);
}

void pMR::ofi::Connection::postSendToPassive()
{
    Tagged message(mPassiveEndpoint.getSendContext(),
        {mPassiveEndpoint.getSendTag()}, {mPeerAddress});
    postSendRequest(
        mEndpoint, message, mEndpoint->checkInjectSize({message.getLength()}));
}

void pMR::ofi::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    Tagged message(memoryRegion, mPassiveEndpoint.getRecvContext(),
        {mPassiveEndpoint.getRecvTag()}, {mPeerAddress});
    postRecvRequest(mEndpoint, message);
}

void pMR::ofi::Connection::postSendToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    Tagged message(memoryRegion, {sizeByte}, mActiveEndpoint.getSendContext(),
        {mActiveEndpoint.getSendTag()}, {mPeerAddress});
    postSendRequest(mEndpoint, message, mEndpoint->checkInjectSize({sizeByte}));
}
#endif // OFI_RMA

void pMR::ofi::Connection::pollActiveSend()
{
    return mActiveEndpoint.pollSend();
}

void pMR::ofi::Connection::pollActiveRecv()
{
    return mActiveEndpoint.pollRecv();
}

void pMR::ofi::Connection::pollPassiveSend()
{
    return mPassiveEndpoint.pollSend();
}

void pMR::ofi::Connection::pollPassiveRecv()
{
    return mPassiveEndpoint.pollRecv();
}
