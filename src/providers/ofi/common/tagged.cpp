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

#include "tagged.hpp"

pMR::ofi::Tagged::Tagged(
    fi_context *context, std::uint64_t tag, fi_addr_t address)
{
    mIOV.iov_base = nullptr;
    mIOV.iov_len = 0;

    mTagged.msg_iov = &mIOV;
    mTagged.desc = 0;
    mTagged.iov_count = 0;
    mTagged.addr = address;
    mTagged.tag = tag;
    mTagged.ignore = 0;
    mTagged.context = context;
}

pMR::ofi::Tagged::Tagged(MemoryRegion &memoryRegion, fi_context *context,
    std::uint64_t tag, fi_addr_t address)
    : Tagged(memoryRegion, memoryRegion.getLength(), context, tag, address)
{
}

pMR::ofi::Tagged::Tagged(MemoryRegion &memoryRegion, std::size_t const sizeByte,
    fi_context *context, std::uint64_t tag, fi_addr_t address)
{
    if(sizeByte == 0)
    {
        mTagged.iov_count = 0;
    }
    else
    {
        mTagged.iov_count = 1;
        mIOV.iov_base = memoryRegion.getBuffer();
        mIOV.iov_len = {sizeByte};
    }

    mTagged.msg_iov = &mIOV;
    mTagged.desc = memoryRegion.getDescriptor();
    mTagged.addr = address;
    mTagged.tag = tag;
    mTagged.ignore = 0;
    mTagged.context = context;
}

fi_msg_tagged *pMR::ofi::Tagged::get()
{
    return &mTagged;
}

fi_msg_tagged const *pMR::ofi::Tagged::get() const
{
    return &mTagged;
}

std::size_t pMR::ofi::Tagged::getLength() const
{
    return {mIOV.iov_len};
}
