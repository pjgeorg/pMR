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

#ifndef pMR_PROVIDERS_OFI_MSG_ENDPOINT_H
#define pMR_PROVIDERS_OFI_MSG_ENDPOINT_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "../common/completionqueue.hpp"
#include "../common/domain.hpp"
#include "../common/info.hpp"
#include "eventqueue.hpp"

namespace pMR
{
    namespace ofi
    {
        class Endpoint
        {
        public:
            Endpoint(Domain &domain, Info &info);
            Endpoint(Endpoint const &) = delete;
            Endpoint(Endpoint &&) = delete;
            Endpoint &operator=(Endpoint const &) = delete;
            Endpoint &operator=(Endpoint &&) = delete;
            ~Endpoint();
            fid_ep *get();
            fid_ep const *get() const;
            void bind(CompletionQueue &queue, std::uint64_t flags);
            void bind(EventQueue &queue);
            void enable();
            void connect(std::vector<std::uint8_t> const &address);
            void accept();
            std::vector<std::uint8_t> getAddress() const;
            std::vector<std::uint8_t> getPeerAddress() const;
            void setAddress(std::vector<std::uint8_t> address);
            fi_context *getSendContext();
            fi_context *getRecvContext();

        private:
            fid_ep *mEndpoint = nullptr;
            fi_context mContext = {};
            fi_context mSendContext = {};
            fi_context mRecvContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_MSG_ENDPOINT_H
