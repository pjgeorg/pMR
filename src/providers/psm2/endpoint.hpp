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

#ifndef pMR_PROVIDERS_PSM2_ENDPOINT_H
#define pMR_PROVIDERS_PSM2_ENDPOINT_H

#include <cstdint>
extern "C" {
#include <psm2.h>
}

namespace pMR
{
    namespace psm2
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

            psm2_ep_t get() const;

            std::uint64_t getNetworkID();
            std::uint64_t getContext();

            psm2_epid_t getID() const;
            psm2_epaddr_t connect(psm2_epid_t const);

        private:
            psm2_ep_t mEndpoint;
            psm2_epid_t mEndpointID;
        };
    }
}
#endif // pMR_PROVIDERS_PSM2_ENDPOINT_H
