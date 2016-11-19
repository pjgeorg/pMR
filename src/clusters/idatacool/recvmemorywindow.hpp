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

#ifndef pMR_CLUSTERS_IDATACOOL_RECVMEMORYWINDOW_H
#define pMR_CLUSTERS_IDATACOOL_RECVMEMORYWINDOW_H

#include <cstdint>
#include <memory>
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
            void *const mBuffer;
            std::uint32_t mSizeByte;
            Provider mProvider = Provider::null;
            std::unique_ptr<null::RecvMemoryWindow,
                null::RecvMemoryWindowDeleter> mNull;
            std::unique_ptr<self::RecvMemoryWindow,
                self::RecvMemoryWindowDeleter> mSelf;
            std::unique_ptr<cma::RecvMemoryWindow,
                cma::RecvMemoryWindowDeleter> mCMA;
            std::unique_ptr<verbs::RecvMemoryWindow,
                verbs::RecvMemoryWindowDeleter> mVerbs;
    };
}
#endif // pMR_CLUSTERS_IDATACOOL_RECVMEMORYWINDOW_H
