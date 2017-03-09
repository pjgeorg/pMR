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

#ifndef pMR_PROVIDERS_VERBS_ENDPOINT_H
#define pMR_PROVIDERS_VERBS_ENDPOINT_H

#include <cstdint>
#include "connectionaddress.hpp"
#include "context.hpp"
#include "protectiondomain.hpp"
#include "queuepair.hpp"

namespace pMR
{
    namespace Verbs
    {
        class Endpoint
        {
        public:
            Endpoint(Context &context, ProtectionDomain &protectionDomain);
            Endpoint(Endpoint const &) = delete;
            Endpoint(Endpoint &&) = delete;
            Endpoint &operator=(Endpoint const &) = delete;
            Endpoint &operator=(Endpoint &&) = delete;
            ~Endpoint() = default;

            QueuePair &getQueuePair();
            QueuePair const &getQueuePair() const;

            void setStateINIT(std::uint8_t const portNumber);
            void setStateRTR(
                std::uint8_t const portNumber, ConnectionAddress const &);
            void setStateRTS();

            void poll();

        private:
            CompletionQueue mCompletionQueue;
            QueuePair mQueuePair;
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_ENDPOINT_H
