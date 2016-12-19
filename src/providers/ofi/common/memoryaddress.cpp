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

#include "memoryaddress.hpp"

pMR::ofi::MemoryAddress::MemoryAddress(MemoryRegion const &memoryRegion)
{
    set(memoryRegion);
}

void pMR::ofi::MemoryAddress::set(MemoryRegion const &memoryRegion)
{
    std::get<0>(mData) = {memoryRegion.getAddress()};
    std::get<1>(mData) = {memoryRegion.getKey()};
    std::get<2>(mData) = {memoryRegion.getLength()};
}

std::uint64_t pMR::ofi::MemoryAddress::getAddress() const
{
    return {std::get<0>(mData)};
}

std::uint64_t pMR::ofi::MemoryAddress::getKey() const
{
    return {std::get<1>(mData)};
}

std::size_t pMR::ofi::MemoryAddress::getLength() const
{
    return {static_cast<std::size_t>(std::get<2>(mData))};
}

std::uint64_t *pMR::ofi::MemoryAddress::rawData()
{
    return mData.data();
}

std::uint64_t const *pMR::ofi::MemoryAddress::rawData() const
{
    return mData.data();
}

std::size_t pMR::ofi::MemoryAddress::size() const
{
    return {sizeof(mData)};
}
