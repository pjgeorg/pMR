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
//!     This file is only required to generate common global reduction
//!     documentation, so that documentation has not to be added to every
//!     specific global reduction implementation.
//!
//! @author Peter Georg
#ifndef pMR_ALLREDUCE_H
#define pMR_ALLREDUCE_H

#include <algorithm>
#include "misc/allocator.hpp"
#include "communicator.hpp"
#include "config.hpp"

#ifdef pMR_ALLREDUCE_MPI
#include "allreduce/mpi/allreduce.hpp"
#endif // pMR_ALLREDUCE_MPI
#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
#include "allreduce/recursivedoubling/allreduce.hpp"
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING

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
        AllReduce(Communicator const &communicator, size_type const sizeByte);
        AllReduce(AllReduce const &) = delete;
        AllReduce(AllReduce &&) = delete;
        AllReduce &operator=(AllReduce const &) = delete;
        AllReduce &operator=(AllReduce &&) = delete;
        ~AllReduce() = default;

        //! @brief Copy data to internal buffer.
        //! @details Copies data from storage - inputIt iterator points to -
        //!     to internal buffer.
        //! @param inputIt Iterator to source.
        //! @param count Number of elements to copy.
        template<class Iterator>
        void insert(Iterator inputIt, size_type const count);

        //! @brief Copy data from internal buffer.
        //! @details Copies data from the internal buffer to the location
        //!     specified by outputIt.
        //! @param outputIt Iterator to destination.
        //! @param count Number of elements to copy.
        template<class Iterator>
        void extract(Iterator outputIt, size_type const count);

        //! @brief Direct access to the internal buffer.
        //! @return Returns pointer to the internal buffer.
        template<typename T = void>
        T *data();
        //! @brief Direct access to the internal buffer.
        //! @return Returns pointer to the internal buffer.
        template<typename T = void>
        T const *data() const;
        //! @brief Get the size of the internal buffer, and hence the
        //!     maximum message size, in Byte.
        //! @return Actual size of the internal buffer in Byte.
        size_type size() const;
        //! @brief Get the size of the internal buffer, and hence the
        //!     maximum message size, in count of type T.
        //! @return Actual size of the internal buffer in count of T.
        template<typename T = unsigned char>
        size_type count() const;

        //! @brief Execute the global reduction using the supplied reduce
        //!     function. Afterwards all process have the result stored in
        //!     their local internal buffers.
        //! @param op Reduce function used in reduction operation.
        //! @param count Number of element of type T to be part of global
        //!     reduction.
        //! @warning The size of all elements of type T to be part of global
        //!     reduction might not exceed the size as returned by size().
        //! @warning The content of the internal buffer that was not
        //!     required for the global reduction is undefined.
        template<typename T>
        void execute(Operation op, size_type const count);

        //! @brief Execute the global reduction using the user-defined reduce
        //!     function. Afterwards all process have the result stored in
        //!     their local internal buffers.
        //! @param op Reduce function used in reduction operation.
        //! @param count Number of element of type T to be part of global
        //!     reduction.
        //! @note Example op function:
        //! @code
        //! void sum(float const *in, float *inout, pMR::size_type count)
        //! {
        //!     for(auto i = decltype(count){0}; i != count; ++i)
        //!     {
        //!        inout[i] += in[i];
        //!     }
        //! }
        //! @endcode
        //! @warning The size of all elements of type T to be part of global
        //!     reduction might not exceed the size as returned by size().
        //! @warning The content of the internal buffer that was not
        //!     required for the global reduction is undefined.
        //! @warning User-defined reduce functions might not be able to use
        //!     best performing routines.
        //! @warning User-defined reduce functions are assumed to be
        //!     associative and commutative.
        template<typename T>
        void execute(void (*op)(T const *in, T *inout, size_type const count),
            size_type const count);

        //! @brief Execute the global reduction using the supplied reduce
        //!     function. Afterwards all process have the result stored in
        //!     their local internal buffers. Results are guaranteed to be bit
        //!     identical.
        //! @param op Reduce function used in reduction operation.
        //! @param count Number of element of type T to be part of global
        //!     reduction.
        //! @note The bit identical reduction is in general slower. Only use
        //!     when it is a requirement to have bit identical results.
        //! @warning The size of all elements of type T to be part of global
        //!     reduction might not exceed the size as returned by size().
        //! @warning The content of the internal buffer that was not
        //!     required for the global reduction is undefined.
        template<typename T>
        void executeBit(Operation op, size_type const count);

        //! @brief Execute the global reduction using the user-defined reduce
        //!     function. Afterwards all process have the result stored in
        //!     their local internal buffers. Results are guaranteed to be bit
        //!     identical.
        //! @param op Reduce function used in reduction operation.
        //! @param count Number of element of type T to be part of global
        //!     reduction.
        //! @note The bit identical reduction is in general slower. Only use
        //!     when it is a requirement to have bit identical results.
        //! @note Example op function:
        //! @code
        //! void sum(float const *in, float *inout, pMR::size_type count)
        //! {
        //!     for(auto i = decltype(count){0}; i != count; ++i)
        //!     {
        //!        inout[i] += in[i];
        //!     }
        //! }
        //! @endcode
        //! @warning The size of all elements of type T to be part of global
        //!     reduction might not exceed the size as returned by size().
        //! @warning The content of the internal buffer that was not
        //!     required for the global reduction is undefined.
        //! @warning User-defined reduce functions might not be able to use
        //!     best performing routines.
        //! @warning User-defined reduce functions are assumed to be
        //!     associative and commutative.
        template<typename T>
        void executeBit(
            void (*op)(T const *in, T *inout, size_type const count),
            size_type const count);

    private:
        size_type const mSizeByte;
        std::vector<unsigned char, pMR::Allocator<unsigned char>> mBuffer;
#ifdef pMR_ALLREDUCE_MPI
        MPI::AllReduce mMPI;
#endif // pMR_ALLREDUCE_MPI
#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
        RecursiveDoubling::AllReduce mRecursiveDoubling;
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING

        template<typename T>
        void checkBuffer(size_type const count);
    };
}

template<class Iterator>
void pMR::AllReduce::insert(Iterator inputIt, size_type const count)
{
    checkBuffer<typename std::iterator_traits<Iterator>::value_type>(count);
    std::copy_n(inputIt, count,
        reinterpret_cast<typename std::iterator_traits<Iterator>::value_type *>(
            mBuffer.data()));
}

template<class Iterator>
void pMR::AllReduce::extract(Iterator outputIt, size_type const count)
{
    checkBuffer<typename std::iterator_traits<Iterator>::value_type>(count);
    std::copy_n(
        reinterpret_cast<
            typename std::iterator_traits<Iterator>::value_type const *>(
            mBuffer.data()),
        count, outputIt);
}

template<typename T>
T *pMR::AllReduce::data()
{
    return reinterpret_cast<T *>(mBuffer.data());
}

template<typename T>
T const *pMR::AllReduce::data() const
{
    return reinterpret_cast<T const *>(mBuffer.data());
}

template<typename T>
pMR::size_type pMR::AllReduce::count() const
{
    return {static_cast<size_type>(mSizeByte / sizeof(T))};
}

template<typename T>
void pMR::AllReduce::execute(Operation op, size_type const count)
{
#ifdef pMR_ALLREDUCE_MPI
    if(mMPI.execute<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_MPI

#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
    if(mRecursiveDoubling.execute<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING

    throw std::runtime_error("pMR: No suitable AllReduce algorithm");
}

template<typename T>
void pMR::AllReduce::execute(
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
#ifdef pMR_ALLREDUCE_MPI
    if(mMPI.execute<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_MPI

#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
    if(mRecursiveDoubling.execute<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING

    throw std::runtime_error("pMR: No suitable AllReduce algorithm");
}

template<typename T>
void pMR::AllReduce::executeBit(Operation op, size_type const count)
{
#ifdef pMR_ALLREDUCE_MPI
    if(mMPI.executeBit<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_MPI

#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
    if(mRecursiveDoubling.executeBit<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING

    throw std::runtime_error("pMR: No suitable AllReduce algorithm");
}

template<typename T>
void pMR::AllReduce::executeBit(
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
#ifdef pMR_ALLREDUCE_MPI
    if(mMPI.executeBit<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_MPI

#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
    if(mRecursiveDoubling.executeBit<T>(mBuffer.data(), op, count))
    {
        return;
    }
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING

    throw std::runtime_error("pMR: No suitable AllReduce algorithm");
}

template<typename T>
void pMR::AllReduce::checkBuffer(size_type const count)
{
    static_assert(std::is_trivial<T>::value,
        "pMR: AllReduce template type is not trivial.");

    if(count > AllReduce::count<T>())
    {
        throw std::out_of_range("pMR: AllReduce buffer");
    }
}
#endif // pMR_ALLREDUCE_H
