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

#include "rma.hpp"

pMR::ofi::RMA::RMA(MemoryRegion &memoryRegion,
    MemoryAddress const &remoteMemoryAddress, fi_context *context,
    std::uint64_t const data, fi_addr_t const address)
    : RMA(memoryRegion, memoryRegion.getLength(), remoteMemoryAddress, context,
          data, address)
{
}

pMR::ofi::RMA::RMA(MemoryRegion &memoryRegion, std::size_t const sizeByte,
    MemoryAddress const &remoteMemoryAddress, fi_context *context,
    std::uint64_t const data, fi_addr_t const address)
{
    if(sizeByte == 0)
    {
        mRMA.iov_count = 0;
    }
    else
    {
        mRMA.iov_count = 1;
        mIOV.iov_base = memoryRegion.getBuffer();
        mIOV.iov_len = {sizeByte};
    }

    if(remoteMemoryAddress.getLength() == 0)
    {
#ifdef OFI_RMA_EVENT_NONZERO
        mRMA.rma_iov_count = 1;
        mRMAIOV.len = 1;
        mRMAIOV.key = {remoteMemoryAddress.getKey()};
#else
        mRMA.rma_iov_count = 0;
#endif // OFI_RMA_EVENT
    }
    else
    {
        mRMA.rma_iov_count = 1;
        mRMAIOV.len = {remoteMemoryAddress.getLength()};
        mRMAIOV.key = {remoteMemoryAddress.getKey()};
    }

#ifdef OFI_MR_SCALABLE
    mRMAIOV.addr = {0};
#else
    mRMAIOV.addr = {remoteMemoryAddress.getAddress()};
#endif // OFI_MR_SCALABLE

    mRMA.msg_iov = &mIOV;
    mRMA.desc = {memoryRegion.getDescriptor()};
    mRMA.rma_iov = &mRMAIOV;
    mRMA.addr = {address};
    mRMA.context = context;
    mRMA.data = {data};
}

fi_msg_rma *pMR::ofi::RMA::get()
{
    return &mRMA;
}

fi_msg_rma const *pMR::ofi::RMA::get() const
{
    return &mRMA;
}

std::size_t pMR::ofi::RMA::getLength() const
{
    return {mIOV.iov_len};
}
