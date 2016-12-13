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

#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_SWITCH_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_SWITCH_H

#include <cstdint>
#include "../context.hpp"
#include "../device.hpp"

namespace pMR
{
    namespace verbs
    {
        std::uint16_t getSwitchLID(
            Device const &, std::uint8_t const portNumber);
        std::uint16_t getSwitchLID(Context &, std::uint8_t const portNumber);
    }
}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_SWITCH_H
