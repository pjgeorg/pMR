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

#ifndef pMR_PROVIDERS_SCIF_ADDRESS_H
#define pMR_PROVIDERS_SCIF_ADDRESS_H

#include <cstdint>
extern "C" {
#include <scif.h>
}

namespace pMR
{
    namespace scif
    {
        class Address
        {
        public:
            Address() = default;
            explicit Address(std::uint16_t port);
            Address(std::uint16_t node, std::uint16_t port);
            ~Address() = default;
            scif_portID *get();
            scif_portID const *get() const;
            std::uint16_t getNode() const;
            std::uint16_t getPort() const;
            void setNode(std::uint16_t const node);
            void setPort(std::uint16_t const port);
            bool operator==(Address const &other) const;
            bool operator!=(Address const &other) const;

        private:
            scif_portID mPortID;
        };
    }
}
#endif // pMR_PROVIDERS_SCIF_ADDRESS_H
