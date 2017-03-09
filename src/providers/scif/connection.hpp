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

#ifndef pMR_PROVIDERS_SCIF_CONNECTION_H
#define pMR_PROVIDERS_SCIF_CONNECTION_H

#include <array>
#include <memory>
#include <config.hpp>
#include "endpoint.hpp"
#include "memoryregion.hpp"
#include "peerendpoint.hpp"

namespace pMR
{
    class Target;

    namespace scif
    {
        class Connection
        {
        public:
            Connection(Target const &target);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection();

            Endpoint &getActiveEndpoint();
            Endpoint const &getActiveEndpoint() const;
            PeerEndpoint &getPassiveEndpoint();
            PeerEndpoint const &getPassiveEndpoint() const;

            void postAddress(off_t const localAddress);
            off_t pollAddress();

            void writeData(MemoryRegion const &, std::size_t const sizeByte);
            void pollLocalNotify();
            void pollRemoteNotify();

        private:
            Endpoint mActiveEndpoint;
            PeerEndpoint mPassiveEndpoint;
            alignas(cAlignment)
                std::array<off_t, cacheLineSize<off_t>()> mTarget = {};
            alignas(cAlignment) std::array<std::uint64_t,
                cacheLineSize<std::uint64_t>()> mLocalNotify = {};
            alignas(cAlignment) std::array<std::uint64_t,
                cacheLineSize<std::uint64_t>()> mRemoteNotify = {};
            std::unique_ptr<MemoryRegion> mTargetMR;
            std::unique_ptr<MemoryRegion> mLocalNotifyMR;
            std::unique_ptr<MemoryRegion> mRemoteNotifyMR;
            off_t *mTargetRemote;
            off_t mNotifyRemote;

            void pollNotify(
                std::array<std::uint64_t, cacheLineSize<std::uint64_t>()>
                    &notify);
        };
    }
}
#endif // pMR_PROVIDERS_SCIF_CONNECTION_H
