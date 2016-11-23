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

#include "scattergather.hpp"

pMR::verbs::ScatterGatherElement::ScatterGatherElement(
        MemoryRegion const& memoryRegion)
{
    mScatterGatherElement.addr = {memoryRegion.getAddress()};
    mScatterGatherElement.length = {memoryRegion.getLength()};
    mScatterGatherElement.lkey = {memoryRegion.getLKey()};
}

pMR::verbs::ScatterGatherElement::ScatterGatherElement(
        MemoryRegion const& memoryRegion, std::uint32_t const sizeByte)
{
    mScatterGatherElement.addr = {memoryRegion.getAddress()};
    mScatterGatherElement.length = {sizeByte};
    mScatterGatherElement.lkey = {memoryRegion.getLKey()};
}

pMR::verbs::ScatterGatherElement::ScatterGatherElement(
        void *address, std::uint32_t const sizeByte)
{
    mScatterGatherElement.addr = {reinterpret_cast<std::uint64_t>(address)};
    mScatterGatherElement.length = {sizeByte};
}

ibv_sge* pMR::verbs::ScatterGatherElement::get()
{
    return &mScatterGatherElement;
}

ibv_sge const* pMR::verbs::ScatterGatherElement::get() const
{
    return &mScatterGatherElement;
}

std::uint32_t pMR::verbs::ScatterGatherElement::getLength() const
{
    return {mScatterGatherElement.length};
}

int pMR::verbs::ScatterGatherElement::getNumEntries() const
{
    if(mScatterGatherElement.length == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
