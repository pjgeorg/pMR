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
#include "../../providers/mpi/memorywindow.hpp"

pMR::RecvMemoryWindow::RecvMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mBuffer(buffer),
        mSizeByte(sizeByte)
{
    mMPI = std::unique_ptr<mpi::RecvMemoryWindow, mpi::RecvMemoryWindowDeleter>(
            new mpi::RecvMemoryWindow(connection.mMPI, buffer, sizeByte));
}

pMR::RecvMemoryWindow::~RecvMemoryWindow() { }

void pMR::RecvMemoryWindow::init()
{
    mMPI->init();
}

void pMR::RecvMemoryWindow::post()
{
    mMPI->post();
}

void pMR::RecvMemoryWindow::wait()
{
    mMPI->wait();
}

void* pMR::RecvMemoryWindow::data()
{
    return mBuffer;
}

void const* pMR::RecvMemoryWindow::data() const
{
    return mBuffer;
}

std::uint32_t pMR::RecvMemoryWindow::size() const
{
    return mSizeByte;
}
