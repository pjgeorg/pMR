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

#include "gid.hpp"
#include <stdexcept>
#include "verbs.hpp"

pMR::Verbs::GID::GID(Context &context, std::uint8_t const portNumber)
{
    if(ibv_query_gid(context.get(), portNumber, cSGIDIndex, &mGID))
    {
        throw std::runtime_error("pMR: Unable to query GID.");
    }
}

pMR::Verbs::GID::GID(ibv_gid const gID) : mGID(gID)
{
}

void pMR::Verbs::GID::setGUID(std::uint64_t const GUID)
{
    mGID.global.interface_id = {GUID};
}

void pMR::Verbs::GID::setSubnetPrefix(std::uint64_t const subnetPrefix)
{
    mGID.global.subnet_prefix = {subnetPrefix};
}

ibv_gid const &pMR::Verbs::GID::get() const
{
    return mGID;
}

std::uint64_t pMR::Verbs::GID::getGUID() const
{
    return {mGID.global.interface_id};
}

std::uint64_t pMR::Verbs::GID::getSubnetPrefix() const
{
    return {mGID.global.subnet_prefix};
}
