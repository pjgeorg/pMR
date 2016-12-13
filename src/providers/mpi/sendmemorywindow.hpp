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

#ifndef pMR_PROVIDERS_MPI_SENDMEMORYWINDOW_H
#define pMR_PROVIDERS_MPI_SENDMEMORYWINDOW_H

#include <memory>
extern "C" {
#include <mpi.h>
}

namespace pMR
{
    namespace mpi
    {
        class Connection;

        class SendMemoryWindow
        {
        public:
            SendMemoryWindow(std::shared_ptr<Connection> const, void *buffer,
                int const sizeByte);
            SendMemoryWindow(SendMemoryWindow const &) = delete;
            SendMemoryWindow(SendMemoryWindow &&) = delete;
            SendMemoryWindow &operator=(SendMemoryWindow const &) = delete;
            SendMemoryWindow &operator=(SendMemoryWindow &&) = delete;
            ~SendMemoryWindow();
            void init();
            void post(int const sizeByte);
            void wait();

        private:
            std::shared_ptr<Connection> const mConnection;
            void *const mBuffer;
            int const mSizeByte;
            MPI_Request mRequest = MPI_REQUEST_NULL;

#ifdef MPI_PERSISTENT
            void initSend();
#endif // MPI_PERSISTENT
            void send(int const sizeByte);
            void freeRequest();
        };
    }
}
#endif // pMR_PROVIDERS_MPI_SENDMEMORYWINDOW_H
