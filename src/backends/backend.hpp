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

#ifndef pMR_BACKENDS_BACKEND_H
#define pMR_BACKENDS_BACKEND_H

#include <array>
#include <cstdint>

namespace pMR
{
    class Target;

    namespace backend
    {
        void sync(Target const &target);
    
        void exchange(Target const &target,
                void const *sendBuffer, void *recvBuffer,
                std::uint32_t const sizeByte);
    
        template<typename T>
        void exchange(Target const &target, T const &sendBuffer, T &recvBuffer)
        {
            return exchange(target, static_cast<void const*>(&sendBuffer),
                    static_cast<void*>(&recvBuffer), sizeof(sendBuffer));
        }

        template<typename T, std::size_t N>
        void exchange(Target const &target,
                std::array<T, N> const &sendBuffer, std::array<T, N> &recvBuffer)
        {
            return exchange(target, sendBuffer.data(),
                    recvBuffer.data(), sizeof(sendBuffer));
        }
    }
}
#endif // pMR_BACKENDS_BACKEND_H
