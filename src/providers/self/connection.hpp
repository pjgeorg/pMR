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

#ifndef pMR_PROVIDERS_SELF_CONNECTION_H
#define pMR_PROVIDERS_SELF_CONNECTION_H

#include <cstdint>

namespace pMR
{
    class Target;

    namespace self
    {
        class Connection
        {
            public:
                Connection(Target const &target);
                Connection(const Connection&) = delete;
                Connection(Connection&&) = delete;
                Connection& operator=(const Connection&) = delete;
                Connection& operator=(Connection&&) = delete;
                ~Connection() = default;
                void postAddress(void *const buffer, std::size_t const sizeByte);
                void pollAddress() const;
                void sendData(void *const buffer, std::size_t const sizeByte);
                void postNotify();
                void pollNotify() const;
            private:
                void **mRemoteBuffer = nullptr;
                std::size_t *mRemoteSizeByte = nullptr;
                void *mDestinationBuffer = nullptr;
                std::size_t mDestinationSizeByte = 0;

                void checkBufferSizeByte(std::size_t const sizeByte) const;
        };
    }
}
#endif // pMR_PROVIDERS_SELF_CONNECTION_H
