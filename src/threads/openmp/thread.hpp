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

#ifndef pMR_THREADS_OPENMP_THREAD_H
#define pMR_THREADS_OPENMP_THREAD_H

#include "mutex.hpp"
#include "scopedlock.hpp"

namespace pMR
{
    namespace thread
    {
        constexpr bool isThreaded();
        constexpr bool isSerialized();
    }
}

constexpr bool pMR::thread::isThreaded()
{
    return {true};
}

constexpr bool pMR::thread::isSerialized()
{
    return {false};
}
#endif // pMR_THREADS_OPENMP_THREAD_H
