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

#ifndef pMR_PROVIDERS_PSM2_DIRECT_SOFTENDPOINT_H
#define pMR_PROVIDERS_PSM2_DIRECT_SOFTENDPOINT_H

#include <cstdint>
extern "C" {
#include <psm2.h>
#include <psm2_mq.h>
}
#include "globalendpoint.hpp"

namespace pMR
{
    namespace psm2
    {
        class SoftEndpoint
        {
        public:
            SoftEndpoint();
            SoftEndpoint(SoftEndpoint const &) = delete;
            SoftEndpoint(SoftEndpoint &&) = delete;
            SoftEndpoint &operator=(SoftEndpoint const &) = delete;
            SoftEndpoint &operator=(SoftEndpoint &&) = delete;
            ~SoftEndpoint() = default;

            psm2_epid_t getID() const;
            void connect(psm2_epid_t const);

            psm2_mq_req_t postRecv(
                void *buffer, std::uint32_t sizeByte, std::uint64_t const rTag);
            psm2_mq_req_t postSend(
                void const *buffer, std::uint32_t sizeByte, std::uint64_t sTag);
            void poll(psm2_mq_req_t &);

        private:
            GlobalEndpoint *mEndpoint = nullptr;
            psm2_epaddr_t mRemoteEndpoint = nullptr;
        };
    }
}
#endif // pMR_PROVIDERS_PSM2_DIRECT_SOFTENDPOINT_H
