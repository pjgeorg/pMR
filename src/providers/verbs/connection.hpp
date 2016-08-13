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
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"
#include "protectiondomain.hpp"
#include "completionqueue.hpp"
#include "queuepair.hpp"
#include "connectionaddress.hpp"
#include "memoryaddress.hpp"
#include "memoryregion.hpp"

namespace pMR { namespace verbs
{
    class Connection
    {
        public:
            Connection(Target const &target,
                    Device const &device, std::uint8_t const portNumber = 1);
            Connection(const Connection&) = delete;
            Connection(Connection&&) = delete;
            Connection& operator=(const Connection&) = delete;
            Connection& operator=(Connection&&) = delete;
            ~Connection() = default;
            Context& getContext();
            Context const& getContext() const;
            ProtectionDomain& getProtectionDomain();
            ProtectionDomain const& getProtectionDomain() const;
            void setLocalMemoryAddress(MemoryRegion const&);
            MemoryAddress const& getRemoteMemoryAddress() const;

            void postSendAddrRequestToPassive();
            void postRecvAddrRequestToActive();
            void postSendDataRequestToActive(MemoryRegion const &memoryRegion,
                    std::uint32_t const sizeByte);
            void postRecvDataRequestToPassive(MemoryRegion const &memoryRegion);
            void postRDMAWriteRequestToActive(MemoryRegion const &memoryRegion,
                    std::uint32_t const sizeByte,
                    MemoryAddress const &remoteMemoryAddress);
            void postRecvRDMARequestToPassive();

            void pollActiveCompletionQueue();
            void pollPassiveCompletionQueue();
        private:
            Context mContext;
            ProtectionDomain mProtectionDomain;
            CompletionQueue mActiveCompletionQueue;
            CompletionQueue mPassiveCompletionQueue;
            QueuePair mActiveQueuePair;
            QueuePair mPassiveQueuePair;
            MemoryAddress mLocalMemoryAddress;
            MemoryAddress mRemoteMemoryAddress;
            MemoryRegion mSendMemoryRegion;
            MemoryRegion mRecvMemoryRegion;
            std::uint32_t mMaxInlineDataSize = 0;
            void postRecvRequest(QueuePair &queuePair,
                    ibv_sge *scatterGatherList, int const numEntries);
            void postSendRequest(QueuePair &queuePair,
                    ibv_sge *scatterGatherList, int const numEntries);
    };

    class ScatterGatherList
    {
        public:
            ScatterGatherList(MemoryRegion const&);
            ScatterGatherList(MemoryRegion const&,
                    std::uint32_t const sizeByte);
            ibv_sge* get();
            ibv_sge const* get() const;
            std::uint32_t getLength() const;
            int getNumEntries() const;
        private:
            ibv_sge mScatterGatherList;
    };
}}
#endif // pMR_PROVIDERS_VERBS_CONNECTION_H
