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

#ifndef pMR_PROVIDER_H
#define pMR_PROVIDER_H

#include "config.hpp"

namespace pMR
{
    enum class Provider
    {
#ifdef pMR_PROVIDER_CMA
        CMA,
#endif // pMR_PROVIDER_CMA
#ifdef pMR_PROVIDER_MPI
        MPI,
#endif // pMR_PROVIDER_MPI
#ifdef pMR_PROVIDER_NULL
        Null,
#endif // pMR_PROVIDER_NULL
#ifdef pMR_PROVIDER_OFI
        OFI,
#endif // pMR_PROVIDER_OFI
#ifdef pMR_PROVIDER_PSM2
        PSM2,
#endif // pMR_PROVIDER_PSM2
#ifdef pMR_PROVIDER_SCIF
        SCIF,
#endif // pMR_PROVIDER_SCIF
#ifdef pMR_PROVIDER_SELF
        Self,
#endif // pMR_PROVIDER_SELF
#ifdef pMR_PROVIDER_VERBS
        Verbs,
#endif // pMR_PROVIDER_VERBS
    };

#ifdef pMR_PROVIDER_CMA
    namespace CMA
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
    namespace MPI
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
    namespace Null
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
    namespace OFI
    {
        class Info;
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
    namespace PSM2
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
    namespace SCIF
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
    namespace Self
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
    namespace Verbs
    {
        class Device;
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
        public:
            void operator()(SendMemoryWindow *) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
        public:
            void operator()(RecvMemoryWindow *) const;
        };
    }
#endif // pMR_PROVIDER_VERBS
}
#endif // pMR_PROVIDER_H
