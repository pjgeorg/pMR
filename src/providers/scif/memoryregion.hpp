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

#ifndef pMR_PROVIDERS_SCIF_MEMORYREGION_H
#define pMR_PROVIDERS_SCIF_MEMORYREGION_H

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <stdexcept>
extern "C" {
#include <scif.h>
}
#include "../../misc/string.hpp"
#include "endpoint.hpp"
#include "scif.hpp"

namespace pMR
{
    namespace scif
    {
        class MemoryRegion
        {
        public:
            template<typename E>
            MemoryRegion(E &endpoint, void *const buffer,
                std::size_t const size, int access);
            MemoryRegion(MemoryRegion const &) = delete;
            MemoryRegion(MemoryRegion &&) = delete;
            MemoryRegion &operator=(MemoryRegion const &) = delete;
            MemoryRegion &operator=(MemoryRegion &&) = delete;
            ~MemoryRegion();
            off_t get() const;
            std::size_t getLength() const;

        private:
            off_t mMemoryRegion = {-1};
            // Defaults to 1 -> get() returns 0 for zero sized memory regions
            off_t mOffset = {1};
            std::size_t mLength = {0};
            scif_epd_t mEndpoint = static_cast<scif_epd_t>(-1);
        };

        template<typename E>
        MemoryRegion::MemoryRegion(
            E &endpoint, void *const buffer, std::size_t const size, int access)
            : mLength{size}, mEndpoint{endpoint.get()}
        {
            if(mLength > 0)
            {
                mOffset = static_cast<off_t>(
                    reinterpret_cast<std::uintptr_t>(buffer) % cGranularity);
                auto addr = reinterpret_cast<void *>(
                    reinterpret_cast<std::uintptr_t>(buffer) - mOffset);

                mMemoryRegion = scif_register(mEndpoint, addr,
                    granularitySize({mLength + mOffset}), 0, access, 0);

                if(mMemoryRegion == SCIF_REGISTER_FAILED)
                {
                    throw std::runtime_error(
                        toString("pMR: Unable to register memory region:",
                            std::strerror(errno)));
                }
            }
        }
    }
}
#endif // pMR_PROVIDERS_SCIF_MEMORYREGION_H
