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

#include "memoryregion.hpp"
#include "../../misc/print.hpp"

pMR::SCIF::MemoryRegion::~MemoryRegion()
{
    if(mLength > 0)
    {
        if(scif_unregister(mEndpoint, mMemoryRegion,
               granularitySize({mLength + mOffset})) == -1)
        {
            print("pMR: Probably unable to unregister memory region:",
                std::strerror(errno));
        }
    }
}

off_t pMR::SCIF::MemoryRegion::get() const
{
    return {mMemoryRegion + mOffset};
}

std::size_t pMR::SCIF::MemoryRegion::getLength() const
{
    return {mLength};
}
