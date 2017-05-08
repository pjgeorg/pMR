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

#include "recvmemorywindow.hpp"
#include "connection.hpp"

pMR::OFI::RecvMemoryWindow::RecvMemoryWindow(
    std::shared_ptr<Connection> const connection, void *buffer,
    std::size_t const sizeByte)
    : mConnection(connection)
    , mMemoryRegion(mConnection->getDomain(), buffer, {sizeByte},
#ifdef OFI_RMA
          FI_REMOTE_WRITE)
#else
          FI_RECV)
#endif // OFI_RMA

#ifdef OFI_RMA_EVENT
    , mCounter(connection->getDomain())
#endif // OFI_RMA_EVENT
{
    mConnection->checkMessageSize({sizeByte});

#ifdef OFI_RMA_EVENT
    mMemoryRegion.bind(mCounter);
#endif // OFI_RMA_EVENT
}

void pMR::OFI::RecvMemoryWindow::init()
{
#ifdef OFI_RMA
#ifdef OFI_RMA_TARGET_RX
    mConnection->postRecvToPassive();
#endif // OFI_RMA_TARGET_RX
    mConnection->setLocalTargetMemoryAddress(mMemoryRegion);
#ifdef OFI_RMA_CONTROL
    mConnection->postWriteAddressToPassive();
#else
    mConnection->postSendAddressToPassive();
#endif // OFI_RMA_CONTROL
#else
    mConnection->postRecvToPassive(mMemoryRegion);
#ifndef OFI_NO_CONTROL
    mConnection->postSendToPassive();
#endif // !OFI_NO_CONTROL
#endif // OFI_RMA
}

void pMR::OFI::RecvMemoryWindow::post()
{
}

void pMR::OFI::RecvMemoryWindow::wait()
{
#ifndef OFI_NO_CONTROL
    mConnection->pollPassiveSend();
#endif // !OFI_NO_CONTROL

#ifdef OFI_RMA_EVENT
    mCounter.poll();
#else
    mConnection->pollPassiveRecv();
#endif // OFI_RMA_EVENT
}
