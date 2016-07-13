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

#ifndef pMR_PROVIDERS_VERBS_CONNECTIONADDRESS_H
#define pMR_PROVIDERS_VERBS_CONNECTIONADDRESS_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"
#include "queuepair.hpp"
#include "gid.hpp"

namespace pMR
{
    class Target;
}

namespace pMR { namespace verbs
{
    class QueuePair;
    class ConnectionAddress
    {
        public:
            ConnectionAddress() = default;
            ConnectionAddress(Context&, QueuePair const&,
                    std::uint8_t const portNumber);
            ConnectionAddress(ConnectionAddress const&, QueuePair const&);
            void setQPN(std::uint32_t const);
            void setLID(std::uint16_t const);
            void setGUID(std::uint64_t const);
            void setSubnetPrefix(std::uint64_t const);
            void setMTU(ibv_mtu const);
            std::uint32_t getQPN() const;
            std::uint16_t getLID() const;
            ibv_gid getGID() const;
            std::uint64_t getGUID() const;
            std::uint64_t getSubnetPrefix() const;
            ibv_mtu getMTU() const;
        private:
            std::uint32_t mQPN;
            GID mGID;
            std::uint16_t mLID;
            ibv_mtu mMTU;
    };

    void exchangeConnectionAddress(Target const &target,
            ConnectionAddress const &originActiveAddress,
            ConnectionAddress const &originPassiveAddress,
            ConnectionAddress &targetActiveAddress,
            ConnectionAddress &targetPassiveAddress);
}}
#endif // pMR_PROVIDERS_VERBS_CONNECTIONADDRESS_H
