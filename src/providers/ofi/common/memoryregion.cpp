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
#include <stdexcept>
extern "C" {
#include <rdma/fi_cm.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>
}
#include "../../../misc/print.hpp"

pMR::ofi::MemoryRegion::MemoryRegion(Domain &domain, void *const buffer,
    std::size_t length, std::uint64_t const access)
    : mBuffer(buffer), mLength{length}
{
    if(length == 0)
    {
#ifdef OFI_RMA_EVENT_NONZERO
        mBuffer = &mBuffer;
        length = 1;
#else
        if(mBuffer == nullptr)
        {
            mBuffer = &mBuffer;
        }
#endif // OFI_RMA_EVENT
    }

    int returnValue;
#ifdef OFI_MR_SCALABLE
    static std::uint64_t requestKey = 0;
    do
    {
        returnValue = fi_mr_reg(domain.get(), mBuffer, {length}, {access}, 0,
            {requestKey}, 0, &mMemoryRegion, &mContext);

        ++requestKey;
    } while(returnValue == -FI_ENOKEY);
#else
    returnValue = fi_mr_reg(domain.get(), mBuffer, {length}, {access}, 0, 0, 0,
        &mMemoryRegion, &mContext);
#endif // OFI_MR_SCALABLE

    if(returnValue)
    {
        switch(returnValue)
        {
            case -FI_ENOKEY:
            {
                throw std::runtime_error(
                    "pMR: Unable to register memory region. "
                    "Request key already in use.");
            }
            case -FI_EKEYREJECTED:
            {
                throw std::runtime_error(
                    "pMR: Unable to register memory region. "
                    "Request key is not available.");
            }
            case -FI_EBADFLAGS:
            {
                throw std::runtime_error(
                    "pMR: Unable to register memory region. "
                    "Specified flags unsupported.");
            }
            default:
            {
                throw std::runtime_error(
                    "pMR: Unable to register memory region.");
            }
        }
    }

    mDescriptor = fi_mr_desc(mMemoryRegion);
}

pMR::ofi::MemoryRegion::~MemoryRegion()
{
    if(mMemoryRegion)
    {
        if(fi_close(&mMemoryRegion->fid) == -FI_EBUSY)
        {
            print(
                "pMR: Probably unable to unregister busy memory region. "
                "Continue anyway.");
        }
    }
}

fid_mr *pMR::ofi::MemoryRegion::get()
{
    return mMemoryRegion;
}

fid_mr const *pMR::ofi::MemoryRegion::get() const
{
    return mMemoryRegion;
}

void **pMR::ofi::MemoryRegion::getDescriptor()
{
    return &mDescriptor;
}

void *const *pMR::ofi::MemoryRegion::getDescriptor() const
{
    return &mDescriptor;
}

std::uint64_t pMR::ofi::MemoryRegion::getKey() const
{
    return {fi_mr_key(mMemoryRegion)};
}

std::size_t pMR::ofi::MemoryRegion::getLength() const
{
    return {mLength};
}

std::uint64_t pMR::ofi::MemoryRegion::getAddress() const
{
    return {reinterpret_cast<std::uintptr_t>(mBuffer)};
}

void *pMR::ofi::MemoryRegion::getBuffer()
{
    return mBuffer;
}

void *pMR::ofi::MemoryRegion::getBuffer() const
{
    return mBuffer;
}

void pMR::ofi::MemoryRegion::bind(Counter &counter)
{
    if(fi_mr_bind(mMemoryRegion, &counter.get()->fid, FI_REMOTE_WRITE))
    {
        throw std::runtime_error("pMR: Unable to bind counter to MR.");
    }
}
