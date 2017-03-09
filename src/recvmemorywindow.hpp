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

#ifndef pMR_RECVMEMORYWINDOW_H
#define pMR_RECVMEMORYWINDOW_H

#include <memory>
#include "config.hpp"
#include "provider.hpp"

namespace pMR
{
    class Connection;

    class RecvMemoryWindow
    {
    public:
        RecvMemoryWindow(
            Connection const &, void *buffer, size_type const sizeByte);
        RecvMemoryWindow(RecvMemoryWindow const &) = delete;
        RecvMemoryWindow(RecvMemoryWindow &&) = default;
        RecvMemoryWindow &operator=(RecvMemoryWindow const &) = delete;
        RecvMemoryWindow &operator=(RecvMemoryWindow &&) = default;
        ~RecvMemoryWindow();
        void init();
        void post();
        void wait();
        void *data();
        void const *data() const;
        size_type size() const;

    private:
        void *mBuffer;
        size_type const mSizeByte;
        Provider mProvider;

#ifdef pMR_PROVIDER_CMA
        std::unique_ptr<CMA::RecvMemoryWindow, CMA::RecvMemoryWindowDeleter>
            mCMA;
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        std::unique_ptr<MPI::RecvMemoryWindow, MPI::RecvMemoryWindowDeleter>
            mMPI;
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        std::unique_ptr<Null::RecvMemoryWindow, Null::RecvMemoryWindowDeleter>
            mNull;
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        std::unique_ptr<OFI::RecvMemoryWindow, OFI::RecvMemoryWindowDeleter>
            mOFI;
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        std::unique_ptr<PSM2::RecvMemoryWindow, PSM2::RecvMemoryWindowDeleter>
            mPSM2;
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        std::unique_ptr<SCIF::RecvMemoryWindow, SCIF::RecvMemoryWindowDeleter>
            mSCIF;
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        std::unique_ptr<Self::RecvMemoryWindow, Self::RecvMemoryWindowDeleter>
            mSelf;
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        std::unique_ptr<Verbs::RecvMemoryWindow, Verbs::RecvMemoryWindowDeleter>
            mVerbs;
#endif // pMR_PROVIDER_VERBS
    };
}
#endif // pMR_RECVMEMORYWINDOW_H
