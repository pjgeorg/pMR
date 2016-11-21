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

#ifndef pMR_SENDMEMORYWINDOW_H
#define pMR_SENDMEMORYWINDOW_H

#include <cstdint>
#include <memory>
#include "config.hpp"
#include "provider.hpp"

namespace pMR
{
    class Connection;

    class SendMemoryWindow
    {
        public:
            SendMemoryWindow(Connection const&,
                    void *buffer, std::uint32_t const sizeByte);
            SendMemoryWindow(const SendMemoryWindow&) = delete;
            SendMemoryWindow(SendMemoryWindow&&) = default;
            SendMemoryWindow& operator=(const SendMemoryWindow&) = delete;
            SendMemoryWindow& operator=(SendMemoryWindow&&) = default;
            ~SendMemoryWindow();
            void init();
            void post();
            void post(std::uint32_t const sizeByte);
            void wait();
            void* data();
            void const* data() const;
            std::uint32_t size() const;
        private:
            void *mBuffer;
            std::uint32_t mSizeByte;
            Provider mProvider;

#ifdef pMR_PROVIDER_CMA
            std::unique_ptr<cma::SendMemoryWindow,
                cma::SendMemoryWindowDeleter> mCMA;
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
            std::unique_ptr<mpi::SendMemoryWindow,
                mpi::SendMemoryWindowDeleter> mMPI;
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
            std::unique_ptr<null::SendMemoryWindow,
                null::SendMemoryWindowDeleter> mNull;
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_SELF
            std::unique_ptr<self::SendMemoryWindow,
                self::SendMemoryWindowDeleter> mSelf;
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
            std::unique_ptr<verbs::SendMemoryWindow,
                verbs::SendMemoryWindowDeleter> mVerbs;
#endif // pMR_PROVIDER_VERBS
    };
}
#endif // pMR_SENDMEMORYWINDOW_H
