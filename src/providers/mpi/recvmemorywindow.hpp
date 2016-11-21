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

#ifndef pMR_PROVIDERS_MPI_RECVMEMORYWINDOW_H
#define pMR_PROVIDERS_MPI_RECVMEMORYWINDOW_H

#include <memory>
#include <cstdint>
extern "C"
{
#include <mpi.h>
}

namespace pMR { namespace mpi
{
    class Connection;

    class RecvMemoryWindow
    {
        public:
            RecvMemoryWindow(std::shared_ptr<Connection> const,
                    void *buffer, std::uint32_t const sizeByte);
            RecvMemoryWindow(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow(RecvMemoryWindow&&) = delete;
            RecvMemoryWindow& operator=(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow& operator=(RecvMemoryWindow&&) = delete;
            ~RecvMemoryWindow();
            void init();
            void post();
            void wait();
        private:
            std::shared_ptr<Connection> const mConnection;
            void *const mBuffer;
            std::uint32_t mSizeByte;
            MPI_Request mRequest = MPI_REQUEST_NULL;

#ifdef MPI_PERSISTENT
            void initRecv();
#endif // MPI_PERSISTENT
            void recv();
            void freeRequest();
    };
}}
#endif // pMR_PROVIDERS_MPI_RECVMEMORYWINDOW_H
