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

#include "mmap.hpp"

void pMR::SCIF::munmap(void *addr, std::size_t const size)
{
    auto diff = static_cast<std::size_t>(
        reinterpret_cast<std::uintptr_t>(addr) % cGranularity);
    addr =
        reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(addr) - diff);

    if(scif_munmap(addr, granularitySize({size + diff})) == -1)
    {
        throw std::runtime_error(toString(
            "pMR: Unable to unmap memory region.", std::strerror(errno)));
    }
}
