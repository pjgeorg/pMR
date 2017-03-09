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

#ifndef pMR_PROVIDERS_CMA_CONNECTION_H
#define pMR_PROVIDERS_CMA_CONNECTION_H

#include <array>
#include <cstdint>
extern "C" {
#include <sys/uio.h>
}
#include <config.hpp>

namespace pMR
{
    class Target;

    namespace CMA
    {
        class Connection
        {
        public:
            Connection(Target const &target);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;
            void sendAddress(iovec &buffer) const;
            void sendData(iovec buffer, std::size_t const sizeByte) const;
            void writeData(iovec localBuffer, iovec remoteBuffer) const;
            void postNotifySend() const;
            void pollNotifySend();
            void postNotifyRecv() const;
            void pollNotifyRecv();

        private:
            pid_t mRemotePID;
            iovec mRemoteAddress;
            iovec mDestination;
            iovec mRemoteNotifySend;
            iovec mRemoteNotifyRecv;
            // Occupy a full cache line for notifier to avoid false cache
            // line sharing. However we only use the first element.
            alignas(cAlignment) std::array<std::uint8_t,
                cacheLineSize<std::uint8_t>()> mNotifySend = {};
            alignas(cAlignment) std::array<std::uint8_t,
                cacheLineSize<std::uint8_t>()> mNotifyRecv = {};
            void postNotify(iovec const &) const;
            void pollNotify(
                std::array<std::uint8_t, cacheLineSize<std::uint8_t>()>
                    &notify);
            void checkBufferSize(iovec const &buffer) const;
        };
    }
}
#endif // pMR_PROVIDERS_CMA_CONNECTION_H
