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

#ifndef pMR_PROVIDERS_SCIF_MMAP_H
#define pMR_PROVIDERS_SCIF_MMAP_H

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <stdexcept>
extern "C" {
#include <scif.h>
}
#include "../../misc/string.hpp"
#include "scif.hpp"

namespace pMR
{
    namespace scif
    {
        template<typename T, typename E>
        T *mmap(E &endpoint, off_t offset, std::size_t const size,
            int const access);
        void munmap(void *addr, std::size_t const size);
    }
}

template<typename T, typename E>
T *pMR::scif::mmap(
    E &endpoint, off_t const offset, std::size_t const size, int const access)
{
    auto diff = offset % cGranularity;

    void *pa = scif_mmap(0, granularitySize({size + diff}), access, 0,
        endpoint.get(), offset - diff);

    if(pa == reinterpret_cast<void *>(-1))
    {
        throw std::runtime_error(toString(
            "pmR: Unable to mmap memory region.", std::strerror(errno)));
    }

    return reinterpret_cast<T *>(reinterpret_cast<std::uintptr_t>(pa) + diff);
}

#endif // pMR_PROVIDERS_SCIF_MMAP_H
