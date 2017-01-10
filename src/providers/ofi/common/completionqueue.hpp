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

#ifndef pMR_PROVIDERS_OFI_COMMON_COMPLETIONQUEUE_H
#define pMR_PROVIDERS_OFI_COMMON_COMPLETIONQUEUE_H

#include <cstdint>
extern "C" {
#include <rdma/fabric.h>
#include <rdma/fi_domain.h>
}
#include "domain.hpp"

namespace pMR
{
    namespace ofi
    {
        class CompletionQueue
        {
        public:
            CompletionQueue(CompletionQueue const &) = delete;
            CompletionQueue(CompletionQueue &&) = delete;
            CompletionQueue &operator=(CompletionQueue const &) = delete;
            CompletionQueue &operator=(CompletionQueue &&) = delete;
            ~CompletionQueue();
            fid_cq *get();
            fid_cq const *get() const;

        protected:
            CompletionQueue() = default;
            fid_cq *mCompletionQueue = nullptr;
            fi_context mContext = {};

            void open(Domain &domain, std::size_t const size,
                fi_cq_format const format);
            void poll(void *entry);
        };

        class CompletionQueueContext : public CompletionQueue
        {
        public:
            CompletionQueueContext(Domain &domain, std::size_t size);
            CompletionQueueContext(CompletionQueueContext const &) = delete;
            CompletionQueueContext(CompletionQueueContext &&) = delete;
            CompletionQueueContext &operator=(
                CompletionQueueContext const &) = delete;
            CompletionQueueContext &operator=(
                CompletionQueueContext &&) = delete;
            ~CompletionQueueContext() = default;
            std::uintptr_t poll();
        };

        class CompletionQueueData : public CompletionQueue
        {
        public:
            CompletionQueueData(Domain &domain, std::size_t size);
            CompletionQueueData(CompletionQueueData const &) = delete;
            CompletionQueueData(CompletionQueueData &&) = delete;
            CompletionQueueData &operator=(
                CompletionQueueData const &) = delete;
            CompletionQueueData &operator=(CompletionQueueData &&) = delete;
            ~CompletionQueueData() = default;
            std::pair<std::uintptr_t, std::uint64_t> poll();
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_COMPLETIONQUEUE_H
