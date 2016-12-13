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

#ifndef pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H
#define pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H

extern "C" {
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR
{
    namespace verbs
    {
        class ProtectionDomain
        {
        public:
            ProtectionDomain(Context &);
            ProtectionDomain(ProtectionDomain const &) = delete;
            ProtectionDomain(ProtectionDomain &&) = delete;
            ProtectionDomain &operator=(ProtectionDomain const &) = delete;
            ProtectionDomain &operator=(ProtectionDomain &&) = delete;
            ~ProtectionDomain() noexcept;
            ibv_pd *get();
            ibv_pd const *get() const;

        private:
            ibv_pd *mProtectionDomain = nullptr;
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H
