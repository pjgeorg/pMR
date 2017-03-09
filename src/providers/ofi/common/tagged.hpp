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

#ifndef pMR_PROVIDERS_OFI_COMMON_TAGGED_H
#define pMR_PROVIDERS_OFI_COMMON_TAGGED_H

#include <cstdint>
#include <stdexcept>
extern "C" {
#include <rdma/fi_tagged.h>
}
#include "memoryregion.hpp"

namespace pMR
{
    namespace OFI
    {
        class Tagged
        {
        public:
            explicit Tagged(
                fi_context *context, std::uint64_t tag, fi_addr_t address = 0);
            Tagged(MemoryRegion &, fi_context *context, std::uint64_t tag,
                fi_addr_t address = 0);
            Tagged(MemoryRegion &, std::size_t const sizeByte,
                fi_context *context, std::uint64_t tag, fi_addr_t address = 0);
            ~Tagged() = default;
            fi_msg_tagged *get();
            fi_msg_tagged const *get() const;
            std::size_t getLength() const;

        private:
            struct iovec mIOV = {};
            fi_msg_tagged mTagged = {};
        };

        template<typename T>
        void postSendRequest(T *endpoint, Tagged &, std::uint64_t flags = 0);

        template<typename T>
        void postRecvRequest(T *endpoint, Tagged &, std::uint64_t flags = 0);
    }
}

template<typename T>
void pMR::OFI::postSendRequest(T *endpoint, Tagged &tagged, std::uint64_t flags)
{
    if(fi_tsendmsg(endpoint->get(), tagged.get(), flags))
    {
        throw std::runtime_error("pMR: Unable to post send request.");
    }
}

template<typename T>
void pMR::OFI::postRecvRequest(T *endpoint, Tagged &tagged, std::uint64_t flags)
{
    if(fi_trecvmsg(endpoint->get(), tagged.get(), flags))
    {
        throw std::runtime_error("pMR: Unable to post receive request.");
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_TAGGED_H
