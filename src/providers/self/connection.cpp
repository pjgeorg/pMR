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

#include "connection.hpp"
#include <array>
#include <cstring>
#include <stdexcept>
#include "../../arch/processor.hpp"
#include "../../backends/backend.hpp"

#ifdef SELF_WARN
#include "../../misc/print.hpp"
#endif // SELF_WARN

pMR::Self::Connection::Connection(Target const &target)
{
#ifdef SELF_WARN
    print("pMR: Using provider self.");
#endif // SELF_WARN

    std::array<std::uintptr_t, 2> originAddress, targetAddress;

    std::get<0>(originAddress) = {
        reinterpret_cast<std::uintptr_t>(&mDestinationBuffer)};
    std::get<1>(originAddress) = {
        reinterpret_cast<std::uintptr_t>(&mDestinationSizeByte)};

    Backend::exchange(target, originAddress, targetAddress);

    mRemoteBuffer = reinterpret_cast<void **>(std::get<0>(targetAddress));
    mRemoteSizeByte = {
        reinterpret_cast<std::size_t *>(std::get<1>(targetAddress))};
}

void pMR::Self::Connection::postAddress(
    void *const buffer, std::size_t const sizeByte)
{
    if(sizeByte == 0)
    {
        // Buffer might be nullptr, require any valid value != nullptr
        *mRemoteBuffer = mRemoteSizeByte;
    }
    else
    {
        *mRemoteBuffer = buffer;
    }
    *mRemoteSizeByte = {sizeByte};
}

void pMR::Self::Connection::pollAddress() const
{
    while(mDestinationBuffer == nullptr)
    {
        CPURelax();
    }
}

void pMR::Self::Connection::sendData(
    void *const buffer, std::size_t const sizeByte)
{
    checkBufferSizeByte({sizeByte});
    std::memcpy(mDestinationBuffer, buffer, {sizeByte});
}

void pMR::Self::Connection::postNotify()
{
    mDestinationBuffer = nullptr;
    mDestinationSizeByte = 0;
}

void pMR::Self::Connection::pollNotify() const
{
    while(*mRemoteBuffer != nullptr)
    {
        CPURelax();
    }
}

void pMR::Self::Connection::checkBufferSizeByte(
    std::size_t const sizeByte) const
{
    if(mDestinationSizeByte < sizeByte)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
}
