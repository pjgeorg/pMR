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

#ifndef pMR_PROVIDERS_OFI_COMMON_MEMORYREGION_H
#define pMR_PROVIDERS_OFI_COMMON_MEMORYREGION_H

#include <cstdint>
extern "C" {
#include <rdma/fi_domain.h>
}
#include "counter.hpp"
#include "domain.hpp"

namespace pMR
{
    namespace OFI
    {
        class MemoryRegion
        {
        public:
            MemoryRegion(Domain &domain, void *const buffer, std::size_t length,
                std::uint64_t const access);
            MemoryRegion(MemoryRegion const &) = delete;
            MemoryRegion(MemoryRegion &&) = delete;
            MemoryRegion &operator=(MemoryRegion const &) = delete;
            MemoryRegion &operator=(MemoryRegion &&) = delete;
            ~MemoryRegion();
            fid_mr *get();
            fid_mr const *get() const;
            void **getDescriptor();
            void *const *getDescriptor() const;
            std::uint64_t getKey() const;
            std::size_t getLength() const;
            std::uint64_t getAddress() const;
            void *getBuffer();
            void *getBuffer() const;
            void bind(Counter &counter);

        private:
            void *mBuffer = nullptr;
            fid_mr *mMemoryRegion = nullptr;
            void *mDescriptor = nullptr;
            std::size_t mLength = 0;
            fi_context mContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_MEMORYREGION_H
