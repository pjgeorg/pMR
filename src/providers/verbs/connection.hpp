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

#ifndef pMR_PROVIDERS_VERBS_CONNECTION_H
#define pMR_PROVIDERS_VERBS_CONNECTION_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include <config.hpp>
#include "connectionaddress.hpp"
#include "context.hpp"
#include "endpoint.hpp"
#include "memoryaddress.hpp"
#include "memoryregion.hpp"
#include "protectiondomain.hpp"
#include "scattergather.hpp"

namespace pMR
{
    namespace verbs
    {
        class Connection
        {
        public:
            Connection(Target const &target, Device const &device,
                std::uint8_t const portNumber = 1);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;
            Context &getContext();
            Context const &getContext() const;
            ProtectionDomain &getProtectionDomain();
            ProtectionDomain const &getProtectionDomain() const;

            void postSendToActive(
                MemoryRegion const &memoryRegion, std::uint32_t const sizeByte);
            void postSendToPassive();
            void postRecvToActive();
            void postRecvToPassive(MemoryRegion &memoryRegion);
            void postRecvToPassive();

            void pollActive();
            void pollPassive();

#ifdef VERBS_RDMA
            void setLocalTargetMemoryAddress(MemoryRegion const &);
#ifdef VERBS_RDMA_CONTROL
            void postWriteAddressToPassive();
#else
            void postSendAddressToPassive();
            void postRecvAddressToActive();
#endif // VERBS_RDMA_CONTROL
            void postWriteToActive(
                MemoryRegion const &memoryRegion, std::uint32_t const sizeByte);
#endif // VERBS_RDMA

        private:
            Context mContext;
            ProtectionDomain mProtectionDomain;
            Endpoint mActiveEndpoint;
            Endpoint mPassiveEndpoint;
#ifdef VERBS_RDMA
            alignas(alignment) MemoryAddress mLocalTargetMemoryAddress;
            alignas(alignment) MemoryAddress mRemoteTargetMemoryAddress;
            MemoryRegion mLocalTargetMemoryRegion;
            MemoryRegion mRemoteTargetMemoryRegion;
#ifdef VERBS_RDMA_CONTROL
            MemoryAddress mRemoteMemoryAddress;
#endif // VERBS_RDMA_CONTROL
#endif // VERBS_RDMA
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_CONNECTION_H
