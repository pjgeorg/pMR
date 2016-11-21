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

pMR::verbs::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mMemoryRegion(mConnection->getContext(),
                mConnection->getProtectionDomain(),
                buffer, sizeByte, IBV_ACCESS_LOCAL_WRITE) { }

void pMR::verbs::SendMemoryWindow::init()
{
#ifdef VERBS_RDMA
    mConnection->postRecvAddressToActive();
#else
    mConnection->postRecvToActive();
#endif // VERBS_RDMA
}

void pMR::verbs::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    mConnection->pollActiveCompletionQueue();

#ifdef VERBS_RDMA
    mConnection->postWriteToActive(mMemoryRegion, sizeByte);
#else
    mConnection->postSendToActive(mMemoryRegion, sizeByte);
#endif // VERBS_RDMA
}

void pMR::verbs::SendMemoryWindow::wait()
{
    mConnection->pollActiveCompletionQueue();
}
