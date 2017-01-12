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

#ifndef pMR_PROVIDERS_OFI_MSG_CONNECTION_H
#define pMR_PROVIDERS_OFI_MSG_CONNECTION_H

#include <cstdint>
#include <memory>
#include "config.hpp"
#include "../common/domain.hpp"
#include "../common/fabric.hpp"
#include "../common/memoryaddress.hpp"
#include "../common/memoryregion.hpp"
#include "softendpoint.hpp"

namespace pMR
{
    class Target;

    namespace ofi
    {
        class Connection
        {
        public:
            Connection(Target const &target, Info info);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;

            Domain &getDomain();
            Domain const &getDomain() const;
            void checkMessageSize(std::size_t const size) const;

#ifdef OFI_RMA
            void setLocalTargetMemoryAddress(MemoryRegion const &);
            void postRecvAddressToActive();
            void postSendAddressToPassive();
            void postRecvToPassive();
            void postWriteToActive(
                MemoryRegion &memoryRegion, std::size_t const sizeByte);
#else
            void postRecvToActive();
            void postSendToPassive();
            void postRecvToPassive(MemoryRegion &memoryRegion);
            void postSendToActive(
                MemoryRegion &memoryRegion, std::size_t const sizeByte);
#endif // OFI_RMA

            void pollActiveSend();
            void pollActiveRecv();
            void pollPassiveSend();
            void pollPassiveRecv();

        private:
            Fabric mFabric;
            Domain mDomain;
            std::unique_ptr<SoftEndpoint> mActiveEndpoint;
            std::unique_ptr<SoftEndpoint> mPassiveEndpoint;
#ifdef OFI_RMA
            alignas(alignment) MemoryAddress mLocalMemoryAddress;
            alignas(alignment) MemoryAddress mRemoteMemoryAddress;
            MemoryRegion mSendLocalAddress;
            MemoryRegion mRecvRemoteAddress;
#endif // OFI_RMA
        };
    }
}
#endif // pMR_PROVIDERS_OFI_MSG_CONNECTION_H
