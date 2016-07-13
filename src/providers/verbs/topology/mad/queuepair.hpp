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

#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_QUEUEPAIR_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_QUEUEPAIR_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "../../protectiondomain.hpp"
#include "../../completionqueue.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class QueuePair
    {
        public:
            QueuePair(ProtectionDomain&,
                    CompletionQueue &sendCompletionQueue,
                    CompletionQueue &recvCompletionQueue);
            QueuePair(const QueuePair&) = delete;
            QueuePair(QueuePair&&) = delete;
            QueuePair& operator=(const QueuePair&) = delete;
            QueuePair& operator=(QueuePair&&) = delete;
            ~QueuePair();
            ibv_qp* get();
            ibv_qp const* get() const;
            void setStateINIT(std::uint8_t const portNumber);
            void setStateRTR();
            void setStateRTS();
        private:
            ibv_qp *mQueuePair = nullptr;
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_QUEUEPAIR_H
