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

#include "rmamessage.hpp"

pMR::ofi::RMAMessage::RMAMessage(MemoryRegion &memoryRegion,
    MemoryAddress const &remoteMemoryAddress, fi_context *context,
    fi_addr_t address)
    : RMAMessage(memoryRegion, memoryRegion.getLength(), remoteMemoryAddress,
          context, address)
{
}

pMR::ofi::RMAMessage::RMAMessage(MemoryRegion &memoryRegion,
    std::size_t const sizeByte, MemoryAddress const &remoteMemoryAddress,
    fi_context *context, fi_addr_t address)
{
    mIOV.iov_base = memoryRegion.getBuffer();
    mIOV.iov_len = {sizeByte};

    mRMAIOV.len = {remoteMemoryAddress.getLength()};
    mRMAIOV.key = {remoteMemoryAddress.getKey()};
#ifdef OFI_MR_SCALABLE
    mRMAIOV.addr = 0;
#else
    mRMAIOV.addr = {remoteMemoryAddress.getAddress()};
#endif // OFI_MR_SCALABLE

    mRMAMessage.msg_iov = &mIOV;
    mRMAMessage.desc = {memoryRegion.getDescriptor()};
    mRMAMessage.iov_count = 1;
    mRMAMessage.rma_iov = &mRMAIOV;
    mRMAMessage.rma_iov_count = 1;
    mRMAMessage.addr = address;
    mRMAMessage.context = context;
}

fi_msg_rma *pMR::ofi::RMAMessage::get()
{
    return &mRMAMessage;
}

fi_msg_rma const *pMR::ofi::RMAMessage::get() const
{
    return &mRMAMessage;
}

std::size_t pMR::ofi::RMAMessage::getLength() const
{
    return {mIOV.iov_len};
}
