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
#include "connection.hpp"
#include "../../threads/thread.hpp"

pMR::mpi::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mBuffer(buffer),
        mSizeByte(sizeByte)
{
#ifdef MPI_PERSISTENT
    thread::ScopedLock scopedLock;
    if(MPI_Send_init(mBuffer, mSizeByte, MPI_BYTE, mConnection->getTargetRank(),
                mConnection->getSendTag(), mConnection->getCommunicator(),
                &mRequest) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to init send data (MPI).");
    }
#endif // MPI_PERSISTENT
}

pMR::mpi::SendMemoryWindow::~SendMemoryWindow()
{
    if(mRequest != MPI_REQUEST_NULL)
    {
        thread::ScopedLock scopedLock;
        if(MPI_Request_free(&mRequest) != MPI_SUCCESS)
        {
            throw std::runtime_error("pMR: Unable to free MPI request.");
        }
    }
}

pMR::mpi::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mBuffer(buffer),
        mSizeByte(sizeByte)
{
#ifdef MPI_PERSISTENT
    thread::ScopedLock scopedLock;
    if(MPI_Recv_init(mBuffer, mSizeByte, MPI_BYTE, mConnection->getTargetRank(),
                mConnection->getRecvTag(), mConnection->getCommunicator(),
                &mRequest) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to init receive data (MPI).");
    }
#endif // MPI_PERSISTENT
}

pMR::mpi::RecvMemoryWindow::~RecvMemoryWindow()
{
    if(mRequest != MPI_REQUEST_NULL)
    {
        thread::ScopedLock scopedLock;
        if(MPI_Request_free(&mRequest) != MPI_SUCCESS)
        {
            throw std::runtime_error("pMR: Unable to free MPI request.");
        }
    }
}

void pMR::mpi::SendMemoryWindow::init() { }

void pMR::mpi::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    thread::ScopedLock scopedLock;
#ifndef MPI_PERSISTENT
    if(MPI_Isend(mBuffer, sizeByte, MPI_BYTE, mConnection->getTargetRank(),
                mConnection->getSendTag(), mConnection->getCommunicator(),
                &mRequest) != MPI_SUCCESS)
#else
    if(MPI_Start(&mRequest) != MPI_SUCCESS)
#endif // MPI_PERSISTENT
    {
        throw std::runtime_error("pMR: Unable to send data (MPI).");
    }
}

void pMR::mpi::SendMemoryWindow::wait()
{
    if(thread::isSerialized() || mConnection->multipleThreadSupport())
    {
        if(MPI_Wait(&mRequest, MPI_STATUS_IGNORE) != MPI_SUCCESS)
        {
            throw std::runtime_error("pMR: Unable to (wait) send data (MPI).");
        }
    }
    else
    {
        int flag = false;
        while(!flag)
        {
            thread::ScopedLock scopedLock;
            if(MPI_Test(&mRequest, &flag, MPI_STATUS_IGNORE) != MPI_SUCCESS)
            {
                throw std::runtime_error(
                        "pMR: Unable to (test) send data (MPI).");
            }
        }
    }
}

void pMR::mpi::RecvMemoryWindow::init()
{
    thread::ScopedLock scopedLock;
#ifndef MPI_PERSISTENT
    if(MPI_Irecv(mBuffer, mSizeByte, MPI_BYTE, mConnection->getTargetRank(),
                mConnection->getRecvTag(), mConnection->getCommunicator(),
                &mRequest) != MPI_SUCCESS)
#else
    if(MPI_Start(&mRequest) != MPI_SUCCESS)
#endif // MPI_PERSISTENT
    {
        throw std::runtime_error("pMR: Unable to receive data (MPI).");
    }
}

void pMR::mpi::RecvMemoryWindow::post() { }

void pMR::mpi::RecvMemoryWindow::wait()
{
    if(thread::isSerialized() || mConnection->multipleThreadSupport())
    {
        if(MPI_Wait(&mRequest, MPI_STATUS_IGNORE) != MPI_SUCCESS)
        {
            throw std::runtime_error(
                    "pMR: Unable to (wait) receive data (MPI).");
        }
    }
    else
    {
        int flag = false;
        while(!flag)
        {
            thread::ScopedLock scopedLock;
            if(MPI_Test(&mRequest, &flag, MPI_STATUS_IGNORE) != MPI_SUCCESS)
            {
                throw std::runtime_error(
                        "pMR: Unable to (test) receive data (MPI).");
            }
        }
    }
}
