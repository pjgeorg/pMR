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

//! @file operations.hpp
//! @brief Defines pre-defined reduce oprations for AllReduce.
//!
//! @author Peter Georg
#ifndef pMR_OPERATIONS_H
#define pMR_OPERATIONS_H

#include <algorithm>
#include "config.hpp"

namespace pMR
{
    //! Pre-defined reduce operations.
    enum class Operation
    {
        //! Return the maximum
        Max,
        //! Return the minimum
        Min,
        //! Return the sum
        Sum,
        //! Return the product
        Prod,
    };

    template<typename T>
    void max(T const *in, T *inout, size_type const count)
    {
#pragma omp simd aligned(inout,in:alignment)
        for(auto i = decltype(count){0}; i < count; ++i)
        {
            inout[i] = std::max(inout[i], in[i]);
        }
    }

    template<typename T>
    void min(T const *in, T *inout, size_type const count)
    {
#pragma omp simd aligned(inout,in:alignment)
        for(auto i = decltype(count){0}; i < count; ++i)
        {
            inout[i] = std::min(inout[i], in[i]);
        }
    }

    template<typename T>
    void sum(T const *in, T *inout, size_type const count)
    {
#pragma omp simd aligned(inout,in:alignment)
        for(auto i = decltype(count){0}; i < count; ++i)
        {
            inout[i] += in[i];
        }
    }

    template<typename T>
    void prod(T const *in, T *inout, size_type const count)
    {
#pragma omp simd aligned(inout,in:alignment)
        for(auto i = decltype(count){0}; i < count; ++i)
        {
            inout[i] *= in[i];
        }
    }
}
#endif // pMR_OPERATIONS_H
