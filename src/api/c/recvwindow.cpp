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

#include "recvwindow.hpp"
#include "../../misc/print.hpp"

extern "C" {
#include "pmr.h"

pMR_RecvWindow *pMR_CreateRecvWindow(
    pMR_Connection *connection, void *buffer, pMR::size_type sizeByte)
{
    try
    {
        return reinterpret_cast<pMR_RecvWindow *>(
            new pMR::RecvWindow<unsigned char>(
                *reinterpret_cast<pMR::Connection *>(connection),
                static_cast<unsigned char *>(buffer), {sizeByte}));
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
        return nullptr;
    }
}

void pMR_DestroyRecvWindow(pMR_RecvWindow *recvWindow)
{
    try
    {
        delete reinterpret_cast<pMR::RecvWindow<unsigned char> *>(recvWindow);
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

int pMR_IsSameRecvWindow(
    pMR_RecvWindow *recvWindow, void *buffer, pMR::size_type sizeByte)
{
    try
    {
        return static_cast<int>(
            reinterpret_cast<pMR::RecvWindow<unsigned char> *>(recvWindow)
                ->isSame(static_cast<unsigned char *>(buffer), {sizeByte}));
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 0;
    }
}

void pMR_InitRecv(pMR_RecvWindow *recvWindow)
{
    try
    {
        return reinterpret_cast<pMR::RecvWindow<unsigned char> *>(recvWindow)
            ->init();
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

void pMR_PostRecv(pMR_RecvWindow *recvWindow)
{
    try
    {
        return reinterpret_cast<pMR::RecvWindow<unsigned char> *>(recvWindow)
            ->post();
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

void pMR_WaitRecv(pMR_RecvWindow *recvWindow)
{
    try
    {
        return reinterpret_cast<pMR::RecvWindow<unsigned char> *>(recvWindow)
            ->wait();
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}
}
