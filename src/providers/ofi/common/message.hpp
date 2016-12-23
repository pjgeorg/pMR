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

#ifndef pMR_PROVIDERS_OFI_COMMON_MESSAGE_H
#define pMR_PROVIDERS_OFI_COMMON_MESSAGE_H

#include <cstdint>
#include <stdexcept>
extern "C" {
#include <rdma/fi_endpoint.h>
}
#include "memoryregion.hpp"

namespace pMR
{
    namespace ofi
    {
        class Message
        {
        public:
            explicit Message(fi_context *context, fi_addr_t address = 0);
            Message(MemoryRegion &, fi_context *context, fi_addr_t address = 0);
            Message(MemoryRegion &, std::size_t const sizeByte,
                fi_context *context, fi_addr_t address = 0);
            ~Message() = default;
            fi_msg *get();
            fi_msg const *get() const;
            std::size_t getLength() const;

        private:
            struct iovec mIOV = {};
            fi_msg mMessage = {};
        };

        template<typename T>
        void postSendRequest(T &pEndpoint, Message &, std::uint64_t flags = 0);

        template<typename T>
        void postRecvRequest(T &pEndpoint, Message &, std::uint64_t flags = 0);
    }
}

template<typename T>
void pMR::ofi::postSendRequest(
    T &pEndpoint, Message &message, std::uint64_t flags)
{
    if(fi_sendmsg(pEndpoint->get(), message.get(), flags))
    {
        throw std::runtime_error("pMR: Unable to post send request.");
    }
}

template<typename T>
void pMR::ofi::postRecvRequest(
    T &pEndpoint, Message &message, std::uint64_t flags)
{
    if(fi_recvmsg(pEndpoint->get(), message.get(), flags))
    {
        throw std::runtime_error("pMR: Unable to post receive request.");
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_MESSAGE_H
