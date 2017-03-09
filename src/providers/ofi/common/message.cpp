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

#include "message.hpp"

pMR::OFI::Message::Message(fi_context *context, fi_addr_t address)
{
    mIOV.iov_base = nullptr;
    mIOV.iov_len = 0;

    mMessage.msg_iov = &mIOV;
    mMessage.desc = 0;
    mMessage.iov_count = 0;
    mMessage.addr = address;
    mMessage.context = context;
}

pMR::OFI::Message::Message(
    MemoryRegion &memoryRegion, fi_context *context, fi_addr_t address)
    : Message(memoryRegion, memoryRegion.getLength(), context, address)
{
}

pMR::OFI::Message::Message(MemoryRegion &memoryRegion,
    std::size_t const sizeByte, fi_context *context, fi_addr_t address)
{
    if(sizeByte == 0)
    {
        mMessage.iov_count = 0;
    }
    else
    {
        mMessage.iov_count = 1;
        mIOV.iov_base = memoryRegion.getBuffer();
        mIOV.iov_len = {sizeByte};
    }

    mMessage.msg_iov = &mIOV;
    mMessage.desc = memoryRegion.getDescriptor();
    mMessage.addr = address;
    mMessage.context = context;
}

fi_msg *pMR::OFI::Message::get()
{
    return &mMessage;
}

fi_msg const *pMR::OFI::Message::get() const
{
    return &mMessage;
}

std::size_t pMR::OFI::Message::getLength() const
{
    return {mIOV.iov_len};
}
