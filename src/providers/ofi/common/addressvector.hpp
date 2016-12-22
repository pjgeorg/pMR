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

#ifndef pMR_PROVIDERS_OFI_COMMON_ADDRESSVECTOR_H
#define pMR_PROVIDERS_OFI_COMMON_ADDRESSVECTOR_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "domain.hpp"

namespace pMR
{
    namespace ofi
    {
        class AddressVector
        {
        public:
            AddressVector(Domain &domain);
            AddressVector(AddressVector const &) = delete;
            AddressVector(AddressVector &&) = delete;
            AddressVector &operator=(AddressVector const &) = delete;
            AddressVector &operator=(AddressVector &&) = delete;
            ~AddressVector();
            fid_av *get();
            fid_av const *get() const;
            fi_addr_t add(std::vector<std::uint8_t> const &addr);
            std::string addressString(
                std::vector<std::uint8_t> const &addr) const;

        private:
            fid_av *mAddressVector = nullptr;
            fi_context mContext = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_ADDRESSVECTOR_H
