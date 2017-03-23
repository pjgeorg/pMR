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

#ifndef pMR_PROVIDERS_OFI_COMMON_DOMAIN_H
#define pMR_PROVIDERS_OFI_COMMON_DOMAIN_H

extern "C" {
#include <rdma/fabric.h>
}
#include "fabric.hpp"
#include "info.hpp"

namespace pMR
{
    namespace OFI
    {
        class Domain
        {
        public:
            Domain(Fabric &fabric, Info &info);
            Domain(Domain const &) = delete;
            Domain(Domain &&) = delete;
            Domain &operator=(Domain const &) = delete;
            Domain &operator=(Domain &&) = delete;
            ~Domain();
            fid_domain *get();
            fid_domain const *get() const;
            void checkMessageSize(std::size_t const size) const;
            std::uint64_t checkInjectSize(std::size_t const size) const;

        private:
            fi_context mContext = {};
            fid_domain *mDomain = nullptr;
            std::size_t mMaxSize = 0;
            std::size_t mInjectSize = 0;
        };

        std::uint64_t checkInjectSize(
            std::size_t const size, std::size_t const maxInjectSize);
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_DOMAIN_H
