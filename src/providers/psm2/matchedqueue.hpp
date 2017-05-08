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

#ifndef pMR_PROVIDERS_PSM2_MATCHEDQUEUE_H
#define pMR_PROVIDERS_PSM2_MATCHEDQUEUE_H

extern "C" {
#include <psm2_mq.h>
}
#include "endpoint.hpp"

namespace pMR
{
    namespace PSM2
    {
        class MatchedQueue
        {
        public:
            MatchedQueue(Endpoint &);
            MatchedQueue(MatchedQueue const &) = delete;
            MatchedQueue(MatchedQueue &&) = delete;
            MatchedQueue &operator=(MatchedQueue const &) = delete;
            MatchedQueue &operator=(MatchedQueue &&) = delete;
            ~MatchedQueue();

            psm2_mq_t get() const;

        private:
            psm2_mq_t mMatchedQueue;
        };
    }
}
#endif // pMR_PROVIDERS_PSM2_MATCHEDQUEUE_H
