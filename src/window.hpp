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

//! @file window.hpp
//! @brief Defines the generic Window base class for SendWindow and RecvWindow.
//!
//! @author Peter Georg
#ifndef pMR_WINDOW_H
#define pMR_WINDOW_H

#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "misc/allocator.hpp"
#include "misc/profiling.hpp"
#include "config.hpp"
#include "misc/print.hpp"

namespace pMR
{
    class Connection;

    //! @brief Generic base class for Windows.
    template<typename T>
    class Window
    {
    public:
        Window(T *buffer, size_type const count);
        explicit Window(size_type const count);
        Window(Window const &) = delete;
        Window(Window &&);
        Window &operator=(Window const &) = delete;
        Window &operator=(Window &&) = default;
        ~Window() = default;
        //! @brief Direct access specified element of the internal buffer.
        //! @param index Position of element in internal buffer.
        //! @return Reference to element at position index.
        T &operator[](int const index);
        //! @brief Direct access specified element of the internal buffer.
        //! @param index Position of element in internal buffer.
        //! @return Reference to const to element at position index.
        T const &operator[](int const index) const;
        //! @brief Direct access specified element of the internal buffer
        //!     with bounds checking.
        //! @param index Position of element in internal buffer.
        //! @return Reference to element at position index.
        T &at(int const index);
        //! @brief Direct access specified element of the internal buffer
        //!     with bounds checking.
        //! @param index Position of element in internal buffer.
        //! @return Reference to const to element at position index.
        T const &at(int const index) const;
        //! @brief Returns an iterator to the beginning of the internal
        //!     buffer.
        //! @return Iterator to the first element of the internal buffer.
        T *begin();
        //! @brief Returns an iterator to const to the beginning of the
        //!     internal buffer.
        //! @return Iterator to const to the first element of the internal
        //!     buffer.
        T const *begin() const;
        //! @brief Returns an iterator to const to the beginning of the
        //!     internal buffer.
        //! @return Iterator to const to the first element of the internal
        //!     buffer.
        T const *cbegin() const;
        //! @brief Returns an Iterator to the end of the internal buffer.
        //! @return Iterator to the element following the last element of
        //!     the internal buffer.
        T *end();
        //! @brief Returns an Iterator to to const the end of the internal
        //!     buffer.
        //! @return Iterator to const to the element following the last
        //!     element of the internal buffer.
        T const *end() const;
        //! @brief Returns an Iterator to const to the end of the internal
        //!     buffer.
        //! @return Iterator to const to the element following the last
        //!     element of the internal buffer.
        T const *cend() const;
        //! @brief Returns a reverse iterator to the beginning of the
        //!     internal buffer.
        //! @return Reverse Iterator to the first element of the internal
        //!     buffer.
        T *rbegin();
        //! @brief Returns a reverse iterator to const to the beginning of
        //!     the internal buffer.
        //! @return Reverse Iterator to const to the first element of the
        //!     internal buffer.
        T const *rbegin() const;
        //! @brief Returns a reverse iterator to const to the beginning of
        //!     the internal buffer.
        //! @return Reverse Iterator to const to the first element of the
        //!     internal buffer.
        T const *crbegin() const;
        //! @brief Returns a reverse Iterator to the end of the internal
        //!     buffer.
        //! @return Reverse Iterator to the element following the last
        //!     element of the internal buffer.
        T *rend();
        //! @brief Returns a reverse Iterator to to const the end of the
        //!     internal buffer.
        //! @return Reverse Iterator to const to the element following the
        //!     last element of the internal buffer.
        T const *rend() const;
        //! @brief Returns a reverse Iterator to const to the end of the
        //!     internal buffer.
        //! @return Reverse Iterator to const to the element following the
        //!     last element of the internal buffer.
        T const *crend() const;
        //! @brief Returns the number of elements in the internal buffer.
        //! @return The number of elements in the internal buffer.
        size_type size() const;
        //! @brief Compare current Window with specified input data.
        //! @details Checks whether the current Window is the same as a
        //!     Window with the specified buffer and count.
        //! @param buffer Pointer to contigious send/receive buffer.
        //! @param count Number of elements of type T in buffer.
        //! @return true if the Window is the same, false otherwise.
        bool isSame(T *const buffer, size_type const count);
        //! @brief Compare current Window with specified input data.
        //! @details Checks whether the current Window is suitable for the
        //!     specified count using internal buffering.
        //! @param count Number of elements of type T in buffer.
        //! @return true if the Window is suitable, false otherwise.
        bool isSame(size_type const count);

    protected:
        std::vector<T, Allocator<T>> mVector;
        T *mBuffer;
        size_type const mCount;
        void checkBuffer();
        void checkBufferPointer();
        void checkBufferType();
        void checkBufferSize();
        void checkBoundaries(size_type const offset, size_type const count);
#ifdef pMR_PROFILING
        size_type mSizeByte = 0;
        std::uint64_t mIterations = 0;
        double mTimeInit = 0.0;
        double mTimePost = 0.0;
        double mTimeWait = 0.0;
        double mTimeCopy = 0.0;
        bool mPrintStats = true;
        template<std::size_t N>
        void printStats(char const (&type)[N]);
#endif // pMR_PROFILING
    };
}

template<typename T>
pMR::Window<T>::Window(T *buffer, size_type const count)
    : mBuffer(buffer), mCount{count}
{
#ifdef pMR_PROFILING
    mSizeByte = {static_cast<size_type>(count * sizeof(T))};
#endif // pMR_PROFILING
    checkBuffer();
}

template<typename T>
pMR::Window<T>::Window(size_type const count)
    : mVector(count), mBuffer(mVector.data()), mCount{count}
{
#ifdef pMR_PROFILING
    mSizeByte = {static_cast<size_type>(count * sizeof(T))};
#endif // pMR_PROFILING
    checkBufferType();
    checkBufferSize();
}

template<typename T>
pMR::Window<T>::Window(Window &&other)
    : mVector(std::move(other.mVector))
    , mBuffer(std::move(other.mBuffer))
    , mCount(std::move(other.mCount))
#ifdef pMR_PROFILING
    , mSizeByte(std::move(other.mSizeByte))
    , mIterations(std::move(other.mIterations))
    , mTimeInit(std::move(other.mTimeInit))
    , mTimePost(std::move(other.mTimePost))
    , mTimeWait(std::move(other.mTimeWait))
    , mTimeCopy(std::move(other.mTimeCopy))
{
    other.mPrintStats = {false};
}
#else
{
}
#endif // pMR_PROFILING

template<typename T>
T &pMR::Window<T>::operator[](int const index)
{
    return mVector[index];
}

template<typename T>
T const &pMR::Window<T>::operator[](int const index) const
{
    return mVector[index];
}

template<typename T>
T &pMR::Window<T>::at(int const index)
{
    return mVector.at(index);
}

template<typename T>
T const &pMR::Window<T>::at(int const index) const
{
    return mVector.at(index);
}

template<typename T>
T *pMR::Window<T>::begin()
{
    return mVector.begin();
}

template<typename T>
T const *pMR::Window<T>::begin() const
{
    return mVector.begin();
}

template<typename T>
T const *pMR::Window<T>::cbegin() const
{
    return mVector.cbegin();
}

template<typename T>
T *pMR::Window<T>::end()
{
    return mVector.end();
}

template<typename T>
T const *pMR::Window<T>::end() const
{
    return mVector.end();
}

template<typename T>
T const *pMR::Window<T>::cend() const
{
    return mVector.cend();
}

template<typename T>
T *pMR::Window<T>::rbegin()
{
    return mVector.rbegin();
}

template<typename T>
T const *pMR::Window<T>::rbegin() const
{
    return mVector.rbegin();
}

template<typename T>
T const *pMR::Window<T>::crbegin() const
{
    return mVector.crbegin();
}

template<typename T>
T *pMR::Window<T>::rend()
{
    return mVector.rend();
}

template<typename T>
T const *pMR::Window<T>::rend() const
{
    return mVector.rend();
}

template<typename T>
T const *pMR::Window<T>::crend() const
{
    return mVector.crend();
}

template<typename T>
pMR::size_type pMR::Window<T>::size() const
{
    return {mVector.size()};
}

template<typename T>
bool pMR::Window<T>::isSame(T *const buffer, size_type const count)
{
    if(mCount != count)
    {
        return {false};
    }
    if(mBuffer != buffer)
    {
        return {false};
    }
    return {true};
}

template<typename T>
bool pMR::Window<T>::isSame(size_type const count)
{
    if(mCount != count)
    {
        return {false};
    }
    return {true};
}

template<typename T>
void pMR::Window<T>::checkBuffer()
{
    checkBufferPointer();
    checkBufferType();
    checkBufferSize();
}

template<typename T>
void pMR::Window<T>::checkBufferPointer()
{
    if(mBuffer == nullptr)
    {
        if(mCount != 0)
        {
            std::runtime_error("pMR: Buffer is non-zero and nullptr.");
        }
    }
}

template<typename T>
void pMR::Window<T>::checkBufferType()
{
    static_assert(
        std::is_trivial<T>::value, "pMR: Window template type is not trivial.");
}

template<typename T>
void pMR::Window<T>::checkBufferSize()
{
    if(std::numeric_limits<size_type>::is_signed)
    {
        if(mCount < 0)
        {
            throw std::logic_error("pMR: Negative message size.");
        }
    }

    if(mCount * sizeof(T) > std::numeric_limits<size_type>::max())
    {
        throw std::overflow_error("pMR: Message size");
    }

#ifdef pMR_WARN_ZERO
    if(mCount == 0)
    {
        print("pMR: Using zero sized Window.");
    }
#endif // pMR_WARN_ZERO
}

template<typename T>
void pMR::Window<T>::checkBoundaries(
    size_type const offset, size_type const count)
{
    if(std::numeric_limits<size_type>::max() >
        std::numeric_limits<decltype(mVector.size())>::max())
    {
        if(offset + count > static_cast<size_type>(mVector.size()))
        {
            throw std::out_of_range("pMR: Window buffer");
        }
    }
    else
    {
        if(static_cast<decltype(mVector.size())>(offset + count) >
            mVector.size())
        {
            throw std::out_of_range("pMR: Window buffer");
        }
    }
}

#ifdef pMR_PROFILING
template<typename T>
template<std::size_t N>
void pMR::Window<T>::printStats(char const (&type)[N])
{
    if(mPrintStats)
    {
        std::ostringstream oss;
        oss << "pMR: " << type << " SizeByte " << std::setw(8) << mSizeByte
            << " Init " << std::scientific << mTimeInit << " Post "
            << std::scientific << mTimePost << " Wait " << std::scientific
            << mTimeWait << " Sum " << std::scientific
            << mTimeInit + mTimePost + mTimeWait << " Copy " << mTimeCopy
            << " Iterations " << mIterations << std::endl;
        std::cout << oss.str();
    }
}
#endif // pMR_PROFILING
#endif // pMR_WINDOW_H
