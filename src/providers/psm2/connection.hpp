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

#ifndef pMR_PROVIDERS_PSM2_CONNECTION_H
#define pMR_PROVIDERS_PSM2_CONNECTION_H

#include <cstdint>
extern "C" {
#include <psm2.h>
#include <psm2_mq.h>
}
#include "softendpoint.hpp"

namespace pMR
{
    class Target;

    namespace psm2
    {
        class Connection
        {
        public:
            Connection(Target const &target);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;

            void postRecv(void *buffer, std::uint32_t sizeByte);
            void postSend(void const *buffer, std::uint32_t sizeByte);
            void pollSend();
            void pollRecv();

        private:
            SoftEndpoint mEndpoint;
            std::uint64_t const mSendTag;
            std::uint64_t mRecvTag;
            psm2_mq_req_t mRecvRequest;
            psm2_mq_req_t mSendRequest;
        };
    }
}
#endif // pMR_PROVIDERS_PSM2_CONNECTION_H
