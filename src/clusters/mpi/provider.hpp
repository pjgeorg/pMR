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

#ifndef pMR_CLUSTERS_MPI_PROVIDER_H
#define pMR_CLUSTERS_MPI_PROVIDER_H

namespace pMR
{
    enum class Provider
    {
        null,
        self,
        loop,
        mpi
    };

    namespace null
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
    namespace self
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
    namespace loop
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
    namespace mpi
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
}
#endif // pMR_PROVIDERS_MPI_PROVIDER_H
