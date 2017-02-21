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

#ifndef pMR_PROVIDERS_SCIF_MESSAGE_H
#define pMR_PROVIDERS_SCIF_MESSAGE_H

#include <cerrno>
#include <cstring>
#include <stdexcept>
extern "C" {
#include <scif.h>
}
#include "../../misc/string.hpp"

namespace pMR
{
    namespace scif
    {
        template<typename E, typename T>
        void send(E &endpoint, T &buffer);
        template<typename E, typename T>
        void send(E &endpoint, T *const buffer, std::size_t const size);
        template<typename E, typename T>
        void recv(E &endpoint, T &buffer);
        template<typename E, typename T>
        void recv(E &endpoint, T *const buffer, std::size_t const size);
    }
}

template<typename E, typename T>
void pMR::scif::send(E &endpoint, T &buffer)
{
    send(endpoint, &buffer, {sizeof(buffer)});
}

template<typename E, typename T>
void pMR::scif::send(E &endpoint, T *const buffer, std::size_t const size)
{
    auto ret = scif_send(endpoint.get(), buffer, size, SCIF_SEND_BLOCK);

    if(ret < 0 || static_cast<std::size_t>(ret) != size)
    {
        throw std::runtime_error(
            toString("pMR: Unable to send message:", std::strerror(errno)));
    }
}

template<typename E, typename T>
void pMR::scif::recv(E &endpoint, T &buffer)
{
    recv(endpoint, &buffer, {sizeof(buffer)});
}

template<typename E, typename T>
void pMR::scif::recv(E &endpoint, T *const buffer, std::size_t const size)
{
    auto ret = scif_recv(endpoint.get(), buffer, size, SCIF_RECV_BLOCK);

    if(ret < 0 || static_cast<std::size_t>(ret) != size)
    {
        throw std::runtime_error(
            toString("pMR: Unable to receive message:", std::strerror(errno)));
    }
}
#endif // pMR_PROVIDERS_SCIF_MESSAGE_H
