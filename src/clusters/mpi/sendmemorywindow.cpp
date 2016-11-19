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
#include "../../providers/mpi/memorywindow.hpp"

pMR::SendMemoryWindow::SendMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mBuffer(buffer),
        mSizeByte(sizeByte)
{
    mMPI = std::unique_ptr<mpi::SendMemoryWindow, mpi::SendMemoryWindowDeleter>(
            new mpi::SendMemoryWindow(connection.mMPI, buffer, sizeByte));
}

pMR::SendMemoryWindow::~SendMemoryWindow() { }

void pMR::SendMemoryWindow::init()
{
    mMPI->init();
}

void pMR::SendMemoryWindow::post()
{
    post(mSizeByte);
}

void pMR::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    if(sizeByte > mSizeByte)
    {
        throw std::length_error("pMR: Send message exceed SendWindow.");
    }

    mMPI->post(sizeByte);
}

void pMR::SendMemoryWindow::wait()
{
    mMPI->wait();
}

void* pMR::SendMemoryWindow::data()
{
    return mBuffer;
}

void const* pMR::SendMemoryWindow::data() const
{
    return mBuffer;
}

std::uint32_t pMR::SendMemoryWindow::size() const
{
    return mSizeByte;
}
