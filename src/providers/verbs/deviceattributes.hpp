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

#ifndef pMR_PROVIDERS_VERBS_DEVICEATTRIBUTES_H
#define pMR_PROVIDERS_VERBS_DEVICEATTRIBUTES_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR
{
    namespace verbs
    {
        class DeviceAttributes
        {
        public:
            DeviceAttributes(Context &);
            ~DeviceAttributes() = default;
            std::uint64_t getNodeGUID() const;
            std::uint8_t getPortCount() const;
            std::uint64_t getMaxMemoryRegionSize() const;

        private:
            ibv_device_attr mDeviceAttributes;
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_DEVICEATTRIBUTES_H
