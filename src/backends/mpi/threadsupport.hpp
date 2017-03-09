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

#ifndef pMR_BACKENDS_MPI_THREADSUPPORT_H
#define pMR_BACKENDS_MPI_THREADSUPPORT_H

#include <mutex>
#include <config.hpp>

namespace pMR
{
    namespace Backend
    {
        class ThreadSupport
        {
        public:
            ThreadSupport();
            ~ThreadSupport() = default;
            enum ThreadLevel getLevel() const;
            static std::mutex Mutex;

        private:
            static enum ThreadLevel sLevel;
        };
    }
}
#endif // pMR_BACKENDS_MPI_THREADSUPPORT_H
