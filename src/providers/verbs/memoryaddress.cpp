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

pMR::verbs::MemoryAddress::MemoryAddress(MemoryRegion const &memoryRegion)
{
    set(memoryRegion);
}

void pMR::verbs::MemoryAddress::set(MemoryRegion const &memoryRegion)
{
    std::get<0>(mData) = {memoryRegion.getAddress()};
    std::get<1>(mData) = {memoryRegion.getRKey()};
}

std::uint64_t pMR::verbs::MemoryAddress::getAddress() const
{
    return *reinterpret_cast<std::uint64_t const*>(mData.data());
}

std::uint32_t pMR::verbs::MemoryAddress::getRKey() const
{
    return static_cast<std::uint32_t>(std::get<1>(mData));
}

std::uint64_t* pMR::verbs::MemoryAddress::rawData()
{
    return mData.data();
}

std::uint64_t const* pMR::verbs::MemoryAddress::rawData() const
{
    return mData.data();
}

std::size_t pMR::verbs::MemoryAddress::size() const
{
    return sizeof(mData);
}
