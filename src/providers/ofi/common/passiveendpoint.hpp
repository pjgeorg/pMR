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

#ifndef pMR_PROVIDERS_OFI_COMMON_PASSIVEENDPOINT_H
#define pMR_PROVIDERS_OFI_COMMON_PASSIVEENDPOINT_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "eventqueue.hpp"
#include "fabric.hpp"
#include "info.hpp"

namespace pMR
{
    namespace ofi
    {
        class PassiveEndpoint
        {
        public:
            PassiveEndpoint(Fabric &fabric, Info &info);
            PassiveEndpoint(PassiveEndpoint const &) = delete;
            PassiveEndpoint(PassiveEndpoint &&) = delete;
            PassiveEndpoint &operator=(PassiveEndpoint const &) = delete;
            PassiveEndpoint &operator=(PassiveEndpoint &&) = delete;
            ~PassiveEndpoint();
            fid_pep *get();
            fid_pep const *get() const;
            void bind(EventQueue &queue);
            void listen();
            std::vector<std::uint8_t> getAddress() const;

        private:
            fid_pep *mPassiveEndpoint = nullptr;
            fi_context mContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_PASSIVEENDPOINT_H
