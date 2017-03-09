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

#ifndef pMR_PROVIDERS_PSM2_MESSAGE_H
#define pMR_PROVIDERS_PSM2_MESSAGE_H

#include <cstdint>
extern "C" {
#include <psm2_mq.h>
}

namespace pMR
{
    namespace PSM2
    {
        class MatchedQueue;
        class Endpoint;

        psm2_mq_tag_t psmTag(std::uint64_t const tag);
        psm2_mq_req_t postRecv(MatchedQueue &, psm2_epaddr_t const source,
            void *buffer, std::uint32_t const sizeByte,
            std::uint64_t const rTag);
        psm2_mq_req_t postSend(MatchedQueue &, psm2_epaddr_t const destination,
            void const *buffer, std::uint32_t const sizeByte,
            std::uint64_t const sTag);
        void poll(Endpoint &, psm2_mq_req_t &);
    }
}
#endif // pMR_PROVIDERS_PSM2_MESSAGE_H
