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

#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_ADDRESS_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_ADDRESS_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "../../portattributes.hpp"
#include "../../protectiondomain.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class Address
    {
        public:
            Address(std::uint8_t const portNumber);
            ~Address() = default;
            ibv_ah_attr* get();
            ibv_ah_attr const* get() const;
        protected:
            ibv_ah_attr mAddress = {};
    };

    class SubnetManager : public Address
    {
        public:
            SubnetManager(PortAttributes&, std::uint8_t const portNumber);
            ~SubnetManager() = default;
    };

    class AddressHandle
    {
        public:
            AddressHandle(ProtectionDomain&, Address&);
            AddressHandle(const AddressHandle&) = delete;
            AddressHandle(AddressHandle&&) = delete;
            AddressHandle& operator=(const AddressHandle&) = delete;
            AddressHandle& operator=(AddressHandle&&) = delete;
            ~AddressHandle();
            ibv_ah* get();
            ibv_ah const* get() const;
        private:
            ibv_ah *mAddressHandle = nullptr;
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_ADDRESS_H
