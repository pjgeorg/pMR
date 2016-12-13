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

#ifndef pMR_PROVIDERS_SELF_SENDMEMORYWINDOW_H
#define pMR_PROVIDERS_SELF_SENDMEMORYWINDOW_H

#include <memory>
#include "config.hpp"

namespace pMR
{
    namespace self
    {
        class Connection;

        class SendMemoryWindow
        {
        public:
            SendMemoryWindow(std::shared_ptr<Connection> const, void *buffer,
                std::size_t const sizeByte);
            SendMemoryWindow(SendMemoryWindow const &) = delete;
            SendMemoryWindow(SendMemoryWindow &&) = delete;
            SendMemoryWindow &operator=(SendMemoryWindow const &) = delete;
            SendMemoryWindow &operator=(SendMemoryWindow &&) = delete;
            ~SendMemoryWindow() = default;
            void init();
            void post(std::size_t const sizeByte);
            void wait();

        private:
            std::shared_ptr<Connection> const mConnection;
            void *mBuffer = nullptr;
            std::size_t const mSizeByte = 0;
        };
    }
}
#endif // pMR_PROVIDERS_SELF_SENDMEMORYWINDOW_H
