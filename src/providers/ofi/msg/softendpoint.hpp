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

#ifndef pMR_PROVIDERS_OFI_MSG_SOFTENDPOINT_H
#define pMR_PROVIDERS_OFI_MSG_SOFTENDPOINT_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "../common/completionqueue.hpp"
#include "../common/domain.hpp"
#include "../common/endpoint.hpp"
#include "../common/eventqueue.hpp"
#include "../common/info.hpp"

namespace pMR
{
    namespace OFI
    {
        class SoftEndpoint
        {
        public:
            SoftEndpoint(Domain &domain, Info &info, EventQueue &eventQueue);
            SoftEndpoint(SoftEndpoint const &) = delete;
            SoftEndpoint(SoftEndpoint &&) = delete;
            SoftEndpoint &operator=(SoftEndpoint const &) = delete;
            SoftEndpoint &operator=(SoftEndpoint &&) = delete;
            ~SoftEndpoint() = default;
            fid_ep *get();
            fid_ep const *get() const;

            void enable();
            void connect(std::vector<std::uint8_t> const &address);
            void accept();

            fi_context *getSendContext();
            fi_context *getRecvContext();

            void pollSend();
            void pollRecv();

        private:
            Endpoint mEndpoint;
            CompletionQueueContext mCompletionQueue;
            fi_context mSendContext = {};
            fi_context mRecvContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_MSG_SOFTENDPOINT_H
