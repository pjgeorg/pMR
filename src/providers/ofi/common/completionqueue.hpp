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

extern "C" {
#include <rdma/fabric.h>
}
#include "domain.hpp"

namespace pMR
{
    namespace ofi
    {
        class CompletionQueue
        {
        public:
            CompletionQueue(Domain &domain, std::size_t size);
            CompletionQueue(CompletionQueue const &) = delete;
            CompletionQueue(CompletionQueue &&) = delete;
            CompletionQueue &operator=(CompletionQueue const &) = delete;
            CompletionQueue &operator=(CompletionQueue &&) = delete;
            ~CompletionQueue();
            fid_cq *get();
            fid_cq const *get() const;
            void poll(int count = 1);

        private:
            fid_cq *mCompletionQueue = nullptr;
            fi_context mContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_COMPLETIONQUEUE_H
