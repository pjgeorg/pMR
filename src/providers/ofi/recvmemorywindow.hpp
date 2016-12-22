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

#ifndef pMR_PROVIDERS_OFI_RECVMEMORYWINDOW_H
#define pMR_PROVIDERS_OFI_RECVMEMORYWINDOW_H

#include <cstdint>
#include <memory>
#include "common/counter.hpp"
#include "common/memoryregion.hpp"

namespace pMR
{
    namespace ofi
    {
        class Connection;

        class RecvMemoryWindow
        {
        public:
            RecvMemoryWindow(std::shared_ptr<Connection> const, void *buffer,
                std::size_t const sizeByte);
            RecvMemoryWindow(RecvMemoryWindow const &) = delete;
            RecvMemoryWindow(RecvMemoryWindow &&) = delete;
            RecvMemoryWindow &operator=(RecvMemoryWindow const &) = delete;
            RecvMemoryWindow &operator=(RecvMemoryWindow &&) = delete;
            ~RecvMemoryWindow() = default;
            void init();
            void post();
            void wait();

        private:
            std::shared_ptr<Connection> const mConnection;
            MemoryRegion mMemoryRegion;
#ifdef OFI_RMA_EVENT
            Counter mCounter;
#endif // OFI_RMA_EVENT
        };
    }
}
#endif // pMR_PROVIDERS_OFI_RECVMEMORYWINDOW_H
