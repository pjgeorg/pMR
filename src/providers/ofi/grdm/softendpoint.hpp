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

#ifndef pMR_PROVIDERS_OFI_GRDM_SOFTENDPOINT_H
#define pMR_PROVIDERS_OFI_GRDM_SOFTENDPOINT_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "../common/memoryaddress.hpp"
#include "../common/memoryregion.hpp"
#include "../common/rma.hpp"
#include "../common/tagged.hpp"
#include "globalendpoint.hpp"

namespace pMR
{
    namespace OFI
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

            std::uint64_t getID() const;
            void setRemoteID(std::uint64_t const remoteID);
            std::uint64_t getRemoteID() const;
            std::uint64_t getSendTag() const;
            std::uint64_t getRecvTag() const;

            fi_context *getSendContext();
            fi_context *getRecvContext();

            void postSend(MemoryRegion &memoryRegion, fi_addr_t const address);
            void postSend(MemoryRegion &memoryRegion,
                std::size_t const sizeByte, fi_addr_t const address);
            void postSend(fi_addr_t const address);
            void postSend(Tagged &message);
            void postRecv(MemoryRegion &memoryRegion, fi_addr_t const address);
            void postRecv(fi_addr_t const address);
            void postRecv(Tagged &message);

            void postWrite(MemoryRegion &memoryRegion, MemoryAddress &target,
                fi_addr_t const address);
            void postWrite(MemoryRegion &memoryRegion, MemoryAddress &target,
                std::size_t const sizeByte, fi_addr_t const address);
            void postWrite(RMA &message);

            void pollSend();
            void pollRecv();

        private:
            GlobalEndpoint *mEndpoint = nullptr;
            std::uint64_t mRemoteID = 0;
            fi_context mSendContext = {};
            fi_context mRecvContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_GRDM_SOFTENDPOINT_H
