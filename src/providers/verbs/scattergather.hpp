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

#ifndef pMR_PROVIDERS_VERBS_SCATTERGATHER_H
#define pMR_PROVIDERS_VERBS_SCATTERGATHER_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include "memoryregion.hpp"

namespace pMR
{
    namespace verbs
    {
        class ScatterGatherElement
        {
        public:
            ScatterGatherElement() = default;
            ScatterGatherElement(MemoryRegion const &);
            ScatterGatherElement(
                MemoryRegion const &, std::uint32_t const sizeByte);
            ScatterGatherElement(void *address, std::uint32_t const sizeByte);
            ~ScatterGatherElement() = default;
            ibv_sge *get();
            ibv_sge const *get() const;
            std::uint32_t getLength() const;
            int getNumEntries() const;

        private:
            ibv_sge mScatterGatherElement = {};
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_SCATTERGATHER_H
