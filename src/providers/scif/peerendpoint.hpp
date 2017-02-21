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

#ifndef pMR_PROVIDERS_SCIF_PEERENDPOINT_H
#define pMR_PROVIDERS_SCIF_PEERENDPOINT_H

#include <cstdint>
extern "C" {
#include <scif.h>
}

namespace pMR
{
    namespace scif
    {
        class PeerEndpoint
        {
        public:
            PeerEndpoint() = default;
            PeerEndpoint(scif_epd_t const endpoint);
            PeerEndpoint(PeerEndpoint const &) = delete;
            PeerEndpoint(PeerEndpoint &&) = default;
            PeerEndpoint &operator=(PeerEndpoint const &) = delete;
            PeerEndpoint &operator=(PeerEndpoint &&) = default;
            ~PeerEndpoint() = default;
            bool isConnected() const;
            scif_epd_t &get();
            scif_epd_t const &get() const;

        private:
            scif_epd_t mPeerEndpoint = static_cast<scif_epd_t>(-1);
        };
    }
}
#endif // pMR_PROVIDERS_SCIF_PEERENDPOINT_H
