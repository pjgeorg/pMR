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

pMR::CMA::SendMemoryWindow::SendMemoryWindow(
    std::shared_ptr<Connection> const connection, void *buffer,
    std::size_t const sizeByte)
    : mConnection(connection)
{
    mBuffer.iov_base = buffer;
    mBuffer.iov_len = {sizeByte};
}

void pMR::CMA::SendMemoryWindow::init()
{
}

void pMR::CMA::SendMemoryWindow::post(std::size_t const sizeByte)
{
    mConnection->pollNotifySend();
    mConnection->sendData(mBuffer, {sizeByte});
    mConnection->postNotifyRecv();
}

void pMR::CMA::SendMemoryWindow::wait()
{
}
