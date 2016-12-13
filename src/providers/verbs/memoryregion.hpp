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

#ifndef pMR_PROVIDERS_VERBS_MEMORYREGION_H
#define pMR_PROVIDERS_VERBS_MEMORYREGION_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include "protectiondomain.hpp"

namespace pMR
{
    namespace verbs
    {
        class MemoryRegion
        {
        public:
            MemoryRegion(Context &context, ProtectionDomain &protectionDomain,
                void *buffer, std::uint32_t const size, int access);
            MemoryRegion(MemoryRegion const &) = delete;
            MemoryRegion(MemoryRegion &&) = delete;
            MemoryRegion &operator=(MemoryRegion const &) = delete;
            MemoryRegion &operator=(MemoryRegion &&) = delete;
            ~MemoryRegion();
            ibv_mr *get();
            ibv_mr const *get() const;
            std::uint64_t getAddress() const;
            std::uint32_t getLKey() const;
            std::uint32_t getRKey() const;
            std::uint32_t getLength() const;

        private:
            ibv_mr *mMemoryRegion = nullptr;
            void registerMemoryRegion(ProtectionDomain &, void *buffer,
                std::uint32_t const size, int const access);
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_MEMORYREGION_H
