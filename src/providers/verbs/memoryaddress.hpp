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

#ifndef pMR_PROVIDERS_VERBS_MEMORYADDRESS_H
#define pMR_PROVIDERS_VERBS_MEMORYADDRESS_H

#include <array>
#include <cstdint>
#include "memoryregion.hpp"

namespace pMR
{
    namespace Verbs
    {
        class MemoryAddress
        {
        public:
            MemoryAddress() = default;
            MemoryAddress(MemoryRegion const &memoryRegion);
            void set(MemoryRegion const &);
            std::uint64_t getAddress() const;
            std::uint32_t getRKey() const;
            std::uint64_t *rawData();
            std::uint64_t const *rawData() const;
            std::uint32_t size() const;
            ~MemoryAddress() = default;

        private:
            std::array<std::uint64_t, 2> mData;
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_MEMORYADDRESS_H
