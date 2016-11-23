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

#include "portattributes.hpp"
#include <stdexcept>

pMR::verbs::PortAttributes::PortAttributes(Context &context,
        std::uint8_t const portNumber)
{
    if(ibv_query_port(context.get(), {portNumber}, &mPortAttributes))
    {
        throw std::runtime_error("pMR: Unable to query IB port.");
    }
}

std::uint16_t pMR::verbs::PortAttributes::getLID() const
{
    return {mPortAttributes.lid};
}

ibv_mtu pMR::verbs::PortAttributes::getActiveMTU() const
{
    return {mPortAttributes.active_mtu};
}

ibv_mtu pMR::verbs::PortAttributes::getMaxMTU() const
{
    return {mPortAttributes.max_mtu};
}

std::uint16_t pMR::verbs::PortAttributes::getSubnetManagerLID() const
{
    return {mPortAttributes.sm_lid};
}

std::uint8_t pMR::verbs::PortAttributes::getSubnetManagerServiceLevel() const
{
    return {mPortAttributes.sm_sl};
}

std::uint32_t pMR::verbs::PortAttributes::getMaxMessageSize() const
{
    return {mPortAttributes.max_msg_sz};
}
