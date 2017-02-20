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

#include "sendmemorywindow.hpp"
#include "connection.hpp"

pMR::ofi::SendMemoryWindow::SendMemoryWindow(
    std::shared_ptr<Connection> const connection, void *buffer,
    std::size_t const sizeByte)
    : mConnection(connection)
    , mMemoryRegion(connection->getDomain(), buffer, {sizeByte},
#ifdef OFI_RMA
          FI_WRITE)
#else
          FI_SEND)
#endif // OFI_RMA
{
    mConnection->checkMessageSize({sizeByte});
}

void pMR::ofi::SendMemoryWindow::init()
{
}

void pMR::ofi::SendMemoryWindow::post(std::size_t const sizeByte)
{
#ifdef OFI_RMA
    mConnection->pollActiveRecv();
#else
    if(!mConnection->checkEagerSize(sizeByte))
    {
        mConnection->pollActiveRecv();
    }
#endif // OFI_RMA

#if defined OFI_RMA && !defined OFI_RMA_CONTROL
    mConnection->postRecvAddressToActive();
#elif !defined OFI_RMA || defined OFI_RMA_TARGET_RX
    mConnection->postRecvToActive();
#endif // OFI_RMA & !OFI_RMA_CONTROL // !OFI_RMA || OFI_RMA_TARGET_RX

#ifdef OFI_RMA
    mConnection->postWriteToActive(mMemoryRegion, {sizeByte});
#else
    mConnection->postSendToActive(mMemoryRegion, {sizeByte});
#endif // OFI_RMA

#ifndef OFI_RMA
    if(mConnection->checkEagerSize(sizeByte))
    {
        mConnection->pollActiveRecv();
    }
#endif // !OFI_RMA
}

void pMR::ofi::SendMemoryWindow::wait()
{
    mConnection->pollActiveSend();
}
