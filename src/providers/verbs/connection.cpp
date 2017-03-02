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
#include "operations.hpp"
#include "verbs.hpp"

pMR::verbs::Connection::Connection(
    Target const &target, Device const &device, std::uint8_t const portNumber)
    : mContext(device)
    , mProtectionDomain(mContext)
    , mActiveEndpoint(mContext, mProtectionDomain)
    , mPassiveEndpoint(mContext, mProtectionDomain)
#ifdef VERBS_RDMA
    , mLocalTargetMemoryRegion(mContext, mProtectionDomain,
          mLocalTargetMemoryAddress.rawData(),
          {mLocalTargetMemoryAddress.size()}, IBV_ACCESS_LOCAL_WRITE)
    , mRemoteTargetMemoryRegion(mContext, mProtectionDomain,
          mRemoteTargetMemoryAddress.rawData(),
          {mRemoteTargetMemoryAddress.size()},
#ifdef VERBS_RDMA_CONTROL
          IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE)
#else
          IBV_ACCESS_LOCAL_WRITE)
#endif // VERBS_RDMA_CONTROL
#endif // VERBS_RDMA
{
    mActiveEndpoint.setStateINIT({portNumber});
    mPassiveEndpoint.setStateINIT({portNumber});

    ConnectionAddress originActiveAddress =
        ConnectionAddress(mContext, mActiveEndpoint, {portNumber});
    ConnectionAddress originPassiveAddress =
        ConnectionAddress(mContext, mPassiveEndpoint, {portNumber});

    ConnectionAddress targetActiveAddress;
    ConnectionAddress targetPassiveAddress;

    exchangeConnectionAddress(target, originActiveAddress, originPassiveAddress,
        targetActiveAddress, targetPassiveAddress);

    mActiveEndpoint.setStateRTR(portNumber, targetPassiveAddress);
    mPassiveEndpoint.setStateRTR(portNumber, targetActiveAddress);

    for(int i = 0; i != InitialPostRecv; ++i)
    {
#if defined VERBS_RDMA && !defined VERBS_RDMA_CONTROL
        postRecvAddressToActive();
#else
        postRecvToActive();
#endif // VERBS_RDMA && !VERBS_RDMA_CONTROL
    }

#ifdef VERBS_RDMA_CONTROL
    MemoryAddress localMemoryAddress(mRemoteTargetMemoryRegion);
    backend::exchange(target, localMemoryAddress, mRemoteMemoryAddress);
#else
    pMR::backend::sync(target);
#endif // VERBS_RDMA_CONTROL

    mActiveEndpoint.setStateRTS();
    mPassiveEndpoint.setStateRTS();
}

pMR::verbs::Context &pMR::verbs::Connection::getContext()
{
    return mContext;
}

pMR::verbs::Context const &pMR::verbs::Connection::getContext() const
{
    return mContext;
}

pMR::verbs::ProtectionDomain &pMR::verbs::Connection::getProtectionDomain()
{
    return mProtectionDomain;
}

pMR::verbs::ProtectionDomain const &
pMR::verbs::Connection::getProtectionDomain() const
{
    return mProtectionDomain;
}

void pMR::verbs::Connection::postSendToActive(
    MemoryRegion const &memoryRegion, std::uint32_t const sizeByte)
{
    ScatterGatherElement scatterGatherElement(memoryRegion, {sizeByte});
    postSendRequest(mActiveEndpoint, scatterGatherElement);
}

void pMR::verbs::Connection::postSendToPassive()
{
    ScatterGatherElement scatterGatherElement;
    postSendRequest(mPassiveEndpoint, scatterGatherElement);
}

void pMR::verbs::Connection::postRecvToActive()
{
    ScatterGatherElement scatterGatherElement;
    postRecvRequest(mActiveEndpoint, scatterGatherElement);
}

void pMR::verbs::Connection::postRecvToPassive(MemoryRegion &memoryRegion)
{
    ScatterGatherElement scatterGatherElement(memoryRegion);
    postRecvRequest(mPassiveEndpoint, scatterGatherElement);
}

void pMR::verbs::Connection::postRecvToPassive()
{
    ScatterGatherElement scatterGatherElement;
    postRecvRequest(mPassiveEndpoint, scatterGatherElement);
}

void pMR::verbs::Connection::pollActive()
{
    mActiveEndpoint.poll();
}

void pMR::verbs::Connection::pollPassive()
{
    mPassiveEndpoint.poll();
}

#ifdef VERBS_RDMA
void pMR::verbs::Connection::setLocalTargetMemoryAddress(
    MemoryRegion const &memoryRegion)
{
    mLocalTargetMemoryAddress.set(memoryRegion);
}

#ifdef VERBS_RDMA_CONTROL
void pMR::verbs::Connection::postWriteAddressToPassive()
{
    ScatterGatherElement scatterGatherElement(mLocalTargetMemoryRegion);
    postWriteRequest(
        mPassiveEndpoint, scatterGatherElement, mRemoteMemoryAddress);
}
#else
void pMR::verbs::Connection::postSendAddressToPassive()
{
    ScatterGatherElement scatterGatherElement(mLocalTargetMemoryRegion);
    postSendRequest(mPassiveEndpoint, scatterGatherElement);
}

void pMR::verbs::Connection::postRecvAddressToActive()
{
    ScatterGatherElement scatterGatherElement(mRemoteTargetMemoryRegion);
    postRecvRequest(mActiveEndpoint, scatterGatherElement);
}
#endif // VERBS_RDMA_CONTROL

void pMR::verbs::Connection::postWriteToActive(
    MemoryRegion const &memoryRegion, std::uint32_t const sizeByte)
{
    ScatterGatherElement scatterGatherElement(memoryRegion, {sizeByte});
    postWriteRequest(
        mActiveEndpoint, scatterGatherElement, mRemoteTargetMemoryAddress);
}
#endif // VERBS_RDMA
