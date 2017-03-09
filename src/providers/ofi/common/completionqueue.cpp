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

#include "completionqueue.hpp"
#include <stdexcept>
extern "C" {
#include <rdma/fi_errno.h>
}
#include "../../arch/processor.hpp"

pMR::OFI::CompletionQueue::~CompletionQueue()
{
    if(mCompletionQueue)
    {
        fi_close(&mCompletionQueue->fid);
    }
}

fid_cq *pMR::OFI::CompletionQueue::get()
{
    return mCompletionQueue;
}

fid_cq const *pMR::OFI::CompletionQueue::get() const
{
    return mCompletionQueue;
}

void pMR::OFI::CompletionQueue::open(
    Domain &domain, std::size_t const size, fi_cq_format const format)
{
    fi_cq_attr attr = {};

    attr.size = {size};
    attr.format = {format};
    attr.wait_obj = FI_WAIT_UNSPEC;
    attr.wait_cond = FI_CQ_COND_NONE;

    if(fi_cq_open(domain.get(), &attr, &mCompletionQueue, &mContext))
    {
        throw std::runtime_error("pMR: Unable to create completion queue.");
    }
}

void pMR::OFI::CompletionQueue::poll(void *entry)
{
#ifdef OFI_POLL_SPIN
    decltype(fi_cq_read(mCompletionQueue, entry, 1)) ret;

    do
    {
        ret = fi_cq_read(mCompletionQueue, entry, 1);

        if(ret == 1)
            return;

        CPURelax();
    } while(ret == -FI_EAGAIN || ret == 0);
#else
    if(fi_cq_sread(mCompletionQueue, entry, 1, NULL, -1) != 1)
#endif // OFI_POLL_SPIN

    {
        throw std::runtime_error(
            "pMR: Unable to retrieve Completion queue event.");
    }
}

pMR::OFI::CompletionQueueContext::CompletionQueueContext(
    Domain &domain, std::size_t size)
{
    open(domain, size, FI_CQ_FORMAT_CONTEXT);
}

std::uintptr_t pMR::OFI::CompletionQueueContext::poll()
{
    fi_cq_entry entry;

    CompletionQueue::poll(&entry);

    return {reinterpret_cast<std::uintptr_t>(entry.op_context)};
}

pMR::OFI::CompletionQueueData::CompletionQueueData(
    Domain &domain, std::size_t size)
{
    open(domain, size, FI_CQ_FORMAT_DATA);
}

std::pair<std::uintptr_t, std::uint64_t> pMR::OFI::CompletionQueueData::poll()
{
    fi_cq_data_entry entry;

    CompletionQueue::poll(&entry);

    if(!(entry.flags & FI_REMOTE_CQ_DATA))
    {
        entry.data = 0;
    }

    return std::make_pair(
        reinterpret_cast<std::uintptr_t>(entry.op_context), entry.data);
}
