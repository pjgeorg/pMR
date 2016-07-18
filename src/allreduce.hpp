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

//! @file allreduce.hpp
//! @brief Public interface for global reducation.
//!
//! @author Peter Georg
#ifndef pMR_ALLREDUCE_H
#define pMR_ALLREDUCE_H

#include <cstdint>
#include "communicator.hpp"

namespace pMR
{
    //! @brief Global reduction operations.
    class AllReduce
    {
        public:
            //! @brief Setup environment for global reduction.
            //! @param communicator Communicator used in global reduction.
            //! @param sizeByte Reduce message size.
            //! @note The size might be chosen greater than the actual message
            //!     size to allow re-use of the same environment for various
            //!     messages sizes.
            //! @note The real size might be greater than the requested size to
            //!     allow optimizations, e.g. cache alignment.
            AllReduce(Communicator const communicator,
                    std::uint32_t const sizeByte);
            AllReduce(const AllReduce&) = delete;
            AllReduce(AllReduce&&) = delete;
            AllReduce& operator=(const AllReduce&) = delete;
            AllReduce& operator=(AllReduce&&) = delete;
            ~AllReduce() = default;

            //! @brief Copy data to internal buffer.
            //! @details Copies data from storage - inputIt iterator points to -
            //!     to internal buffer.
            //! @param inputIt Iterator to source.
            //! @param count Number of elements to copy.
            template<typename T, class Iterator>
            void insert(Iterator inputIt, std::uint32_t const count);

            //! @brief Copy data from internal buffer.
            //! @details Copies data from the internal buffer to the location
            //!     specified by outputIt.
            //! @param outputIt Iterator to destination.
            //! @param count Number of elements to copy.
            template<typename T, class Iterator>
            void extract(Iterator outputIt, std::uint32_t const count);

            //! @brief Direct access to the internal buffer.
            //! @return Returns pointer to the internal buffer.
            template<typename T>
            T* data();
            //! @brief Direct access to the internal buffer.
            //! @return Returns pointer to the internal buffer.
            template<typename T>
            T const* data() const;
            //! @brief Get the size of the internal buffer, and hence the
            //!     maximum message size.
            //! @return Actual size of the internal buffer.
            template<typename T>
            std::uint32_t size() const;

            //! @brief Execute the global reduction using the supplied reduce
            //!     function. Afterwards all process have the result stored in
            //!     their local internal buffers.
            //! @param reduce Reduce function used in reduction operation.
            //! @param count Number of element of type T to be part of global
            //!     reduction.
            //! @note Example reduce function:
            //! @code
            //! void plus(float const *in, float *inout, std::uint32_t count)
            //! {
            //!     for(std::uint32_t i = 0; i != count; ++i)
            //!     {
            //!        inout[i] += in[i];
            //!     }
            //! }
            //! @endcode
            //! @warning The size of all elements of type T to be part of global
            //!     reduction might not exceed the size as returned by size().
            //! @warning The content of the internal buffer that was not
            //!     required for the global reduction is undefined.
            template<typename T>
            void execute(void (*reduce)
                    (T const *in, T *inout, std::uint32_t const count),
                    std::uint32_t const count);
    };
}
#endif // pMR_ALLREDUCE_H
