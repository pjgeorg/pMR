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

#ifndef pMR_THREADS_CPP11_MUTEX_H
#define pMR_THREADS_CPP11_MUTEX_H

#include <mutex>

namespace pMR
{
    namespace thread
    {
        class Mutex
        {
        public:
            Mutex() = default;
            Mutex(Mutex const &) = delete;
            Mutex(Mutex &&) = delete;
            Mutex &operator=(Mutex const &) = delete;
            Mutex &operator=(Mutex &&) = delete;
            ~Mutex() = default;
            void lock();
            void unlock();

        private:
            std::mutex mLock;
        };
    }
}
#endif // pMR_THREADS_CPP11_MUTEX_H
