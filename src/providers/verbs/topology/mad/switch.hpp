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

#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_SWITCH_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_SWITCH_H

#include "mad.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class SwitchLID : public MAD
    {
        public:
            SwitchLID(Context&, std::uint8_t const portNumber);
            SwitchLID(const SwitchLID&) = delete;
            SwitchLID(SwitchLID&&) = delete;
            SwitchLID& operator=(const SwitchLID&) = delete;
            SwitchLID& operator=(SwitchLID&&) = delete;
            ~SwitchLID() = default;
            bool validate();
            int getSwitchLID();
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_SWITCH_H
