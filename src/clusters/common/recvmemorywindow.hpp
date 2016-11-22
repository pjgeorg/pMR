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

#include <cstdint>
#include <memory>
#include "config.hpp"
#include "provider.hpp"

namespace pMR
{
    class Connection;

    class RecvMemoryWindow
    {
        public:
            RecvMemoryWindow(Connection const&,
                    void *buffer, std::uint32_t const sizeByte);
            RecvMemoryWindow(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow(RecvMemoryWindow&&) = default;
            RecvMemoryWindow& operator=(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow& operator=(RecvMemoryWindow&&) = default;
            ~RecvMemoryWindow();
            void init();
            void post();
            void wait();
            void* data();
            void const* data() const;
            std::uint32_t size() const;
        private:
            void *mBuffer;
            std::uint32_t mSizeByte;
            Provider mProvider;

#ifdef pMR_PROVIDER_CMA
            std::unique_ptr<cma::RecvMemoryWindow,
                cma::RecvMemoryWindowDeleter> mCMA;
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
            std::unique_ptr<mpi::RecvMemoryWindow,
                mpi::RecvMemoryWindowDeleter> mMPI;
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
            std::unique_ptr<null::RecvMemoryWindow,
                null::RecvMemoryWindowDeleter> mNull;
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
            std::unique_ptr<ofi::RecvMemoryWindow,
                ofi::RecvMemoryWindowDeleter> mOFI;
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_SELF
            std::unique_ptr<self::RecvMemoryWindow,
                self::RecvMemoryWindowDeleter> mSelf;
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
            std::unique_ptr<verbs::RecvMemoryWindow,
                verbs::RecvMemoryWindowDeleter> mVerbs;
#endif // pMR_PROVIDER_VERBS
    };
}
#endif // pMR_RECVMEMORYWINDOW_H
