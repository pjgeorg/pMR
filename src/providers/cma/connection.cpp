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
#include <stdexcept>
extern "C"
{
#include <unistd.h>
}
#include "../../backends/backend.hpp"
#include "../../arch/processor.hpp"

pMR::cma::Connection::Connection(Target const &target)
{
    std::array<std::uint64_t, 7> originAddress, targetAddress;

    std::int64_t pid = {getpid()};
    std::get<0>(originAddress) = {*reinterpret_cast<std::uint64_t*>(&pid)};
    std::get<1>(originAddress) = 
        {reinterpret_cast<std::uintptr_t>(&mDestination)};
    std::get<2>(originAddress) = {sizeof(mDestination)};
    std::get<3>(originAddress)
        = {reinterpret_cast<std::uintptr_t>(mNotifySend)};
    std::get<4>(originAddress) = {sizeof(mNotifySend[0])};
    std::get<5>(originAddress)
        = {reinterpret_cast<std::uintptr_t>(mNotifyRecv)};
    std::get<6>(originAddress) = {sizeof(mNotifyRecv[0])};

    backend::exchange(target, originAddress, targetAddress);

    mRemotePID = {static_cast<int>(*reinterpret_cast<std::int64_t*>
            (&std::get<0>(targetAddress)))};
    mRemoteAddress.iov_base =
        reinterpret_cast<void*>(std::get<1>(targetAddress));
    mRemoteAddress.iov_len =
        {static_cast<std::size_t>(std::get<2>(targetAddress))};
    mRemoteNotifySend.iov_base = 
        reinterpret_cast<void*>(std::get<3>(targetAddress));
    mRemoteNotifySend.iov_len =
        {static_cast<std::size_t>(std::get<4>(targetAddress))};
    mRemoteNotifyRecv.iov_base = 
        reinterpret_cast<void*>(std::get<5>(targetAddress));
    mRemoteNotifyRecv.iov_len =
        {static_cast<std::size_t>(std::get<6>(targetAddress))};
}

void pMR::cma::Connection::sendAddress(iovec &buffer) const
{
    iovec localBuffer;
    localBuffer.iov_base = &buffer;
    localBuffer.iov_len = {sizeof(buffer)};

    writeData(localBuffer, mRemoteAddress);
}

void pMR::cma::Connection::sendData(iovec buffer, std::size_t const sizeByte)
    const
{
    buffer.iov_len = {sizeByte};
    checkBufferSize(buffer);

    writeData(buffer, mDestination);
}

void pMR::cma::Connection::writeData(iovec localBuffer, iovec remoteBuffer)
    const
{
    while(localBuffer.iov_len > 0)
    {
        auto ret = process_vm_writev(mRemotePID, &localBuffer, 1,
                &remoteBuffer, 1, 0);

        if(ret < 0)
        {
            throw std::runtime_error("pMR: CMA failed to write data.");
        }

        localBuffer.iov_base =
            static_cast<void*>(static_cast<char*>(localBuffer.iov_base) + ret);
        localBuffer.iov_len -= ret;
        remoteBuffer.iov_base =
            static_cast<void*>(static_cast<char*>(remoteBuffer.iov_base) + ret);
        remoteBuffer.iov_len -= ret;
    }
}

void pMR::cma::Connection::postNotifySend() const
{
    postNotify(mRemoteNotifySend);
}

void pMR::cma::Connection::pollNotifySend()
{
    pollNotify(mNotifySend);
}

void pMR::cma::Connection::postNotifyRecv() const
{
    postNotify(mRemoteNotifyRecv);
}

void pMR::cma::Connection::pollNotifyRecv()
{
    pollNotify(mNotifyRecv);
}

void pMR::cma::Connection::postNotify(iovec const &remoteNotify) const
{
    std::uint8_t notify = 1;
    iovec localNotify;
    localNotify.iov_base = &notify;
    localNotify.iov_len = {sizeof(notify)};

    writeData(localNotify, remoteNotify);
}

void pMR::cma::Connection::pollNotify(std::uint8_t (&notify)[64])
{
    while(notify[0] == 0)
    {
        CPURelax();
    }
    notify[0] = 0;
}

void pMR::cma::Connection::checkBufferSize(iovec const &buffer) const
{
    if(mDestination.iov_len < buffer.iov_len)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
}
