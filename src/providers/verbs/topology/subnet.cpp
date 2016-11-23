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

#include "../topology.hpp"

std::uint64_t pMR::verbs::getSubnetPrefix(Device const &device,
        std::uint8_t const portNumber)
{
    Context context(device);
    return {getSubnetPrefix(context, {portNumber})};
}

std::uint64_t pMR::verbs::getSubnetPrefix(Context &context,
        std::uint8_t const portNumber)
{
    GID const gID(context, {portNumber});
    return {getSubnetPrefix(gID)};
}

std::uint64_t pMR::verbs::getSubnetPrefix(GID const &gID)
{
    return {gID.getSubnetPrefix()};
}
