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

pMR::ofi::CompletionQueue::~CompletionQueue()
{
    if(mCompletionQueue)
    {
        fi_close(&mCompletionQueue->fid);
    }
}

fid_cq *pMR::ofi::CompletionQueue::get()
{
    return mCompletionQueue;
}

fid_cq const *pMR::ofi::CompletionQueue::get() const
{
    return mCompletionQueue;
}

void pMR::ofi::CompletionQueue::open(
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

void pMR::ofi::CompletionQueue::poll(void *entry)
{
    if(fi_cq_sread(mCompletionQueue, entry, 1, NULL, -1) != 1)
    {
        throw std::runtime_error(
            "pMR: Unable to retrieve Completion queue event.");
    }
}

pMR::ofi::CompletionQueueContext::CompletionQueueContext(
    Domain &domain, std::size_t size)
{
    open(domain, size, FI_CQ_FORMAT_CONTEXT);
}

std::uintptr_t pMR::ofi::CompletionQueueContext::poll()
{
    fi_cq_entry entry;

    CompletionQueue::poll(&entry);

    return {reinterpret_cast<std::uintptr_t>(entry.op_context)};
}

pMR::ofi::CompletionQueueData::CompletionQueueData(
    Domain &domain, std::size_t size)
{
    open(domain, size, FI_CQ_FORMAT_DATA);
}

std::pair<std::uintptr_t, std::uint64_t> pMR::ofi::CompletionQueueData::poll()
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
