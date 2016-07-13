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

#include "memorywindow.hpp"
#include <cstring>
#include <stdexcept>
#include "connection.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT

pMR::self::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection), mBuffer(buffer), mSizeByte(sizeByte) { }

pMR::self::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection), mBuffer(buffer), mSizeByte(sizeByte) { }

void pMR::self::SendMemoryWindow::init() { }

void pMR::self::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    auto vDestinationBuffer =
        reinterpret_cast<void *volatile*>(&mConnection->mDestinationBuffer);
    while(*vDestinationBuffer == nullptr) { }

    if(mConnection->mDestinationSizeByte < sizeByte)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
#ifdef HINT
    print("pMR: Hint: Using self communication. Intentional?");
    if(mConnection->mDestinationSizeByte > sizeByte)
    {
        print("pMR: HINT: RecvWindow larger than SendWindow.");
    }
#endif // HINT

    std::memcpy(mConnection->mDestinationBuffer, mBuffer, sizeByte);

    mConnection->mDestinationBuffer = nullptr;
}

void pMR::self::SendMemoryWindow::wait() { }

void pMR::self::RecvMemoryWindow::init()
{
    *mConnection->mRemoteBuffer = mBuffer;
    *mConnection->mRemoteSizeByte = mSizeByte;
}

void pMR::self::RecvMemoryWindow::post() { }

void pMR::self::RecvMemoryWindow::wait()
{
    auto vRemoteBuffer =
        reinterpret_cast<void *volatile*>(mConnection->mRemoteBuffer);
    while(*vRemoteBuffer != nullptr) { }
}
