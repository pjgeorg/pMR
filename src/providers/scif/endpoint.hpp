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

#ifndef pMR_PROVIDERS_SCIF_ENDPOINT_H
#define pMR_PROVIDERS_SCIF_ENDPOINT_H

#include <cstdint>
extern "C" {
#include <scif.h>
}
#include "address.hpp"
#include "peerendpoint.hpp"

namespace pMR
{
    namespace scif
    {
        class Endpoint
        {
        public:
            Endpoint();
            Endpoint(Endpoint const &) = delete;
            Endpoint(Endpoint &&) = delete;
            Endpoint &operator=(Endpoint const &) = delete;
            Endpoint &operator=(Endpoint &&) = delete;
            ~Endpoint();
            scif_epd_t &get();
            scif_epd_t const &get() const;
            Address bind(std::uint16_t port = 0);
            void listen();
            bool connect(Address &address, bool const blocking = true);
            PeerEndpoint accept(
                Address const &address, bool const blocking = false);

        private:
            scif_epd_t mEndpoint = static_cast<scif_epd_t>(-1);
            void setFileDescriptorBlocking(bool const blocking);
        };
    }
}
#endif // pMR_PROVIDERS_SCIF_ENDPOINT_H
