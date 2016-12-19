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
extern "C" {
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
}
#include "../../backends/backend.hpp"
#include "../common/message.hpp"
#include "../common/rmamessage.hpp"
#include "../ofi.hpp"
#include "passiveendpoint.hpp"

pMR::ofi::Connection::Connection(Target const &target, Info info)
    : mFabric(info)
    , mDomain(mFabric, info)
#ifdef OFI_RMA
    , mSendLocalAddress(mDomain, mLocalMemoryAddress.rawData(),
          {mLocalMemoryAddress.size()}, FI_SEND)
    , mRecvRemoteAddress(mDomain, mRemoteMemoryAddress.rawData(),
          {mRemoteMemoryAddress.size()}, FI_RECV)
#endif // OFI_RMA
    , mActiveCompletionQueue(mDomain, {info.getContextSize()})
    , mPassiveCompletionQueue(mDomain, {info.getContextSize()})
    , mMaxSize{info.maxSize()}
    , mInjectSize{info.injectSize()}
{
    EventQueue eventQueue(mFabric);

    PassiveEndpoint passiveEndpoint(mFabric, info);
    passiveEndpoint.bind(eventQueue);
    passiveEndpoint.listen();

    std::vector<std::uint8_t> localAddress = passiveEndpoint.getAddress();
    std::vector<std::uint8_t> remoteAddress(localAddress.size());
    pMR::backend::exchange(target, localAddress, remoteAddress);

    info.setDestinationAddress(remoteAddress);

    mActiveEndpoint = new Endpoint(mDomain, info);
    mActiveEndpoint->bind(mActiveCompletionQueue, FI_TRANSMIT | FI_RECV);
    mActiveEndpoint->bind(eventQueue);
    mActiveEndpoint->enable();

#ifdef OFI_RMA
    postRecvAddressToActive();
#else
    postRecvToActive();
#endif // OFI_RMA

    mActiveEndpoint->connect(remoteAddress);
    auto connReq = eventQueue.pollConnectionRequest();
    mPassiveEndpoint = new Endpoint(mDomain, connReq);
    mPassiveEndpoint->bind(mPassiveCompletionQueue, FI_TRANSMIT | FI_RECV);
    mPassiveEndpoint->bind(eventQueue);
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
    if(size > mMaxSize)
    {
        throw std::length_error("pMR: Message size overflow.");
    }
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
    postSendRequest(mPassiveEndpoint, message);
}

void pMR::ofi::Connection::postRecvToPassive()
{
    Message message(mPassiveEndpoint->getRecvContext());
    postRecvRequest(mPassiveEndpoint, message);
}

void pMR::ofi::Connection::postWriteToActive(
    MemoryRegion &memoryRegion, std::size_t const sizeByte)
{
    RMAMessage message(memoryRegion, {sizeByte}, mRemoteMemoryAddress,
        mActiveEndpoint->getSendContext());
    postWriteRequest(mActiveEndpoint, message);
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
    postSendRequest(mPassiveEndpoint, message);
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
    postSendRequest(mActiveEndpoint, message);
}
#endif // OFI_RMA

void pMR::ofi::Connection::pollActiveCompletionQueue()
{
    mActiveCompletionQueue.poll();
}

void pMR::ofi::Connection::pollPassiveCompletionQueue()
{
    mPassiveCompletionQueue.poll();
}

std::uint64_t pMR::ofi::Connection::checkInjectSize(std::size_t size) const
{
    if(size <= mInjectSize)
    {
        return FI_INJECT;
    }
    else
    {
        return 0;
    }
}

void pMR::ofi::Connection::postSendRequest(Endpoint *endpoint, Message &message)
{
    if(fi_sendmsg(endpoint->get(), message.get(),
           checkInjectSize({message.getLength()})))
    {
        throw std::runtime_error("pMR: Unable to post send request.");
    }
}

void pMR::ofi::Connection::postRecvRequest(Endpoint *endpoint, Message &message)
{
    if(fi_recvmsg(endpoint->get(), message.get(), 0))
    {
        throw std::runtime_error("pMR: Unable to post receive request.");
    }
}

#ifdef OFI_RMA
void pMR::ofi::Connection::postWriteRequest(
    Endpoint *endpoint, RMAMessage &message)
{
    if(fi_writemsg(endpoint->get(), message.get(),
#ifndef OFI_RMA_EVENT
           FI_REMOTE_CQ_DATA |
#endif // OFI_RMA_EVENT
               checkInjectSize({message.getLength()})))
    {
        throw std::runtime_error("pMR: Unable to post write request.");
    }
}
#endif // OFI_RMA
