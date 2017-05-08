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

#ifndef pMR_PROVIDERS_SCIF_RMA_H
#define pMR_PROVIDERS_SCIF_RMA_H

#include <cerrno>
#include <cstring>
#include <stdexcept>
extern "C" {
#include <scif.h>
}
#include "../../misc/string.hpp"
#include "scif.hpp"

namespace pMR
{
    namespace SCIF
    {
        template<typename E>
        void write(E &endpoint, MemoryRegion const &localBuffer,
            off_t const remoteBuffer);
        template<typename E>
        void write(E &endpoint, MemoryRegion const &localBuffer,
            off_t const remoteBuffer, std::size_t const size);
        template<typename E>
        void fenceSignal(E &endpoint, off_t const localOffset,
            std::uint64_t localValue, off_t remoteOffset,
            std::uint64_t remoteValue);
    }
}

template<typename E>
void pMR::SCIF::write(
    E &endpoint, MemoryRegion const &localBuffer, off_t const remoteBuffer)
{
    write(endpoint, localBuffer, {remoteBuffer}, {localBuffer.getLength()});
}

template<typename E>
void pMR::SCIF::write(E &endpoint, MemoryRegion const &localBuffer,
    off_t const remoteBuffer, std::size_t const size)
{
    int flags = 0;
    if(size < cDMAThreshold)
    {
        flags |= SCIF_RMA_USECPU;
    }

    if(scif_writeto(
           endpoint.get(), localBuffer.get(), size, remoteBuffer, flags) == -1)
    {
        throw std::runtime_error(
            toString("pMR: Unable to write data:", std::strerror(errno)));
    }
}

template<typename E>
void pMR::SCIF::fenceSignal(E &endpoint, off_t const localOffset,
    std::uint64_t localValue, off_t remoteOffset, std::uint64_t remoteValue)
{
    if(scif_fence_signal(endpoint.get(), localOffset, localValue, remoteOffset,
           remoteValue,
           SCIF_FENCE_INIT_SELF | SCIF_SIGNAL_LOCAL | SCIF_SIGNAL_REMOTE) == -1)
    {
        throw std::runtime_error(
            toString("pMR: Unable to fence signal:", std::strerror(errno)));
    }
}
#endif // pMR_PROVIDERS_SCIF_RMA_H
