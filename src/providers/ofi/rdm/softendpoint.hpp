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

#ifndef pMR_PROVIDERS_OFI_RDM_SOFTENDPOINT_H
#define pMR_PROVIDERS_OFI_RDM_SOFTENDPOINT_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "globalendpoint.hpp"

namespace pMR
{
    namespace ofi
    {
        class SoftEndpoint
        {
        public:
            SoftEndpoint(GlobalEndpoint *endpoint);
            SoftEndpoint(SoftEndpoint const &) = delete;
            SoftEndpoint(SoftEndpoint &&) = delete;
            SoftEndpoint &operator=(SoftEndpoint const &) = delete;
            SoftEndpoint &operator=(SoftEndpoint &&) = delete;
            ~SoftEndpoint();

            std::uint64_t getSendTag() const;
            void setRecvTag(std::uint64_t const recvTag);
            std::uint64_t getRecvTag() const;

            fi_context *getSendContext();
            fi_context *getRecvContext();

            void pollSend();
            void pollRecv();

        private:
            GlobalEndpoint *mEndpoint = nullptr;
            std::uint64_t mSendTag = 0;
            std::uint64_t mRecvTag = 0;
            fi_context mSendContext = {};
            fi_context mRecvContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_RDM_SOFTENDPOINT_H
