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
#include <stdexcept>
#include "memoryaddress.hpp"
#include "connection.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT
#include "../../misc/print.hpp"

pMR::verbs::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mMemoryRegion(mConnection->getContext(),
                mConnection->getProtectionDomain(),
                buffer, sizeByte, IBV_ACCESS_LOCAL_WRITE) { }

pMR::verbs::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mMemoryRegion(mConnection->getContext(),
                mConnection->getProtectionDomain(),
                buffer, sizeByte,
                IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE) { }

void pMR::verbs::SendMemoryWindow::init()
{
    mConnection->postRecvAddrRequestToActive();
}

void pMR::verbs::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    // Recv memory address from target 
    mConnection->pollActiveCompletionQueue();
    
    MemoryAddress remoteMemoryAddress = mConnection->getRemoteMemoryAddress();

    if(remoteMemoryAddress.getLength() < sizeByte)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
#ifdef HINT
    if(remoteMemoryAddress.getLength() > sizeByte)
    {
        print("pMR: HINT: RecvWindow larger than SendWindow.");
    }
#endif // HINT

#ifndef VERBS_RDMA
    mConnection->postSendRequestToActive(mMemoryRegion, sizeByte);
#else
    mConnection->postRDMAWriteRequestToActive(mMemoryRegion, sizeByte,
            mConnection->getRemoteMemoryAddress());
#endif // VERBS_RDMA
}

void pMR::verbs::SendMemoryWindow::wait()
{
    mConnection->pollActiveCompletionQueue();
}

void pMR::verbs::RecvMemoryWindow::init()
{
#ifndef VERBS_RDMA
    mConnection->postRecvRequestToPassive(mMemoryRegion);
#else
    mConnection->postRecvRequestToPassive();
#endif // VERBS_RDMA

    // Send memory region to target
    mConnection->setLocalMemoryAddress(mMemoryRegion);
    mConnection->postSendAddrRequestToPassive();
}

void pMR::verbs::RecvMemoryWindow::post() { }

void pMR::verbs::RecvMemoryWindow::wait()
{
    mConnection->pollPassiveCompletionQueue();
    mConnection->pollPassiveCompletionQueue();
}
