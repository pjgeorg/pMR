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

#include "node.hpp"
#include "../../providers/scif/topology.hpp"
#include "../../providers/verbs/topology.hpp"

pMR::Node::Node(pMR::verbs::Device const &device)
{
    verbs::Context context(device);
    mNodeGUID = {verbs::getNodeGUID(context)};
    std::get<0>(mSwitchLID) = {verbs::getSwitchLID(context, 1)};
    std::get<1>(mSwitchLID) = {verbs::getSwitchLID(context, 2)};
    mSCIFNodeID = {scif::getNodeID()};
}

pMR::Node::Node(std::array<std::uint64_t, 4> const &flat)
{
    mNodeGUID = {std::get<0>(flat)};
    std::get<0>(mSwitchLID) = {static_cast<std::uint16_t>(std::get<1>(flat))};
    std::get<1>(mSwitchLID) = {static_cast<std::uint16_t>(std::get<2>(flat))};
    mSCIFNodeID = {static_cast<std::uint16_t>(std::get<3>(flat))};
}

std::array<std::uint64_t, 4> pMR::Node::flatten() const
{
    std::array<std::uint64_t, 4> flat;
    std::get<0>(flat) = {mNodeGUID};
    std::get<1>(flat) = {static_cast<std::uint64_t>(std::get<0>(mSwitchLID))};
    std::get<2>(flat) = {static_cast<std::uint64_t>(std::get<1>(mSwitchLID))};
    std::get<3>(flat) = {static_cast<std::uint64_t>(mSCIFNodeID)};
    return flat;
}

std::uint64_t pMR::Node::getNodeGUID() const
{
    return {mNodeGUID};
}

std::uint16_t pMR::Node::getSwitchLID(std::uint8_t const portNumber) const
{
    return {mSwitchLID.at(portNumber - 1)};
}

std::uint16_t pMR::Node::getSCIFNodeID() const
{
    return {mSCIFNodeID};
}
