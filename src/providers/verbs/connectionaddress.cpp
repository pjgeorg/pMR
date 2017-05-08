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

#include "connectionaddress.hpp"
#include <array>
#include <stdexcept>
#include "../../backends/backend.hpp"
#include "endpoint.hpp"
#include "portattributes.hpp"
#include "queuepair.hpp"
#include "verbs.hpp"

pMR::Verbs::ConnectionAddress::ConnectionAddress(
    Context &context, Endpoint const &endpoint, std::uint8_t const portNumber)
    : ConnectionAddress(context, endpoint.getQueuePair(), {portNumber})
{
}

pMR::Verbs::ConnectionAddress::ConnectionAddress(
    Context &context, QueuePair const &queuePair, std::uint8_t const portNumber)
    : mQPN{queuePair.getQPN()}, mGID{context, portNumber}
{
    mLID = {PortAttributes(context, portNumber).getLID()};
}

pMR::Verbs::ConnectionAddress::ConnectionAddress(
    ConnectionAddress const &connectionAddress, Endpoint const &endpoint)
    : ConnectionAddress(connectionAddress, endpoint.getQueuePair())
{
}

pMR::Verbs::ConnectionAddress::ConnectionAddress(
    ConnectionAddress const &connectionAddress, QueuePair const &queuePair)
    : mQPN{queuePair.getQPN()}
    , mGID{connectionAddress.getGID()}
    , mLID{connectionAddress.getLID()}
{
}

void pMR::Verbs::ConnectionAddress::setQPN(std::uint32_t const QPN)
{
    mQPN = {QPN};
}

void pMR::Verbs::ConnectionAddress::setLID(std::uint16_t const LID)
{
    mLID = {LID};
}

void pMR::Verbs::ConnectionAddress::setGUID(std::uint64_t const GUID)
{
    mGID.setGUID(GUID);
}

void pMR::Verbs::ConnectionAddress::setSubnetPrefix(
    std::uint64_t const subnetPrefix)
{
    mGID.setSubnetPrefix({subnetPrefix});
}

std::uint32_t pMR::Verbs::ConnectionAddress::getQPN() const
{
    return {mQPN};
}

std::uint16_t pMR::Verbs::ConnectionAddress::getLID() const
{
    return {mLID};
}

ibv_gid pMR::Verbs::ConnectionAddress::getGID() const
{
    return mGID.get();
}

std::uint64_t pMR::Verbs::ConnectionAddress::getGUID() const
{
    return {mGID.getGUID()};
}

std::uint64_t pMR::Verbs::ConnectionAddress::getSubnetPrefix() const
{
    return {mGID.getSubnetPrefix()};
}

void pMR::Verbs::exchangeConnectionAddress(pMR::Target const &target,
    ConnectionAddress const &originActiveAddress,
    ConnectionAddress const &originPassiveAddress,
    ConnectionAddress &targetActiveAddress,
    ConnectionAddress &targetPassiveAddress)
{
    std::array<std::uint64_t, 5> sendBuffer = {
        originActiveAddress.getQPN(), originActiveAddress.getLID(),
        originActiveAddress.getSubnetPrefix(), originActiveAddress.getGUID(),
        originPassiveAddress.getQPN(),
    };
    decltype(sendBuffer) recvBuffer;

    pMR::Backend::exchange(target, sendBuffer, recvBuffer);

    targetActiveAddress.setQPN(std::get<0>(recvBuffer));
    targetActiveAddress.setLID(std::get<1>(recvBuffer));
    targetActiveAddress.setSubnetPrefix(std::get<2>(recvBuffer));
    targetActiveAddress.setGUID(std::get<3>(recvBuffer));

    targetPassiveAddress = targetActiveAddress;
    targetPassiveAddress.setQPN(std::get<4>(recvBuffer));
}
