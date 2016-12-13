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

#ifndef pMR_PROVIDERS_VERBS_QUEUEPAIR_H
#define pMR_PROVIDERS_VERBS_QUEUEPAIR_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include "connectionaddress.hpp"
#include "completionqueue.hpp"
#include "protectiondomain.hpp"

namespace pMR
{
    namespace verbs
    {
        class ConnectionAddress;
        class QueuePair
        {
        public:
            QueuePair(ProtectionDomain &, CompletionQueue &sendCompletionQueue,
                CompletionQueue &recvCompletionQueue);
            QueuePair(QueuePair const &) = delete;
            QueuePair(QueuePair &&) = delete;
            QueuePair &operator=(QueuePair const &) = delete;
            QueuePair &operator=(QueuePair &&) = delete;
            ~QueuePair() noexcept;
            ibv_qp *get();
            ibv_qp const *get() const;
            std::uint32_t getQPN() const;
            void setStateINIT(std::uint8_t const portNumber);
            void setStateRTR(
                std::uint8_t const portNumber, ConnectionAddress const &);
            void setStateRTS();

        private:
            ibv_qp *mQueuePair = nullptr;
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_QUEUEPAIR_H
