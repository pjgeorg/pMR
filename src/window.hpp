//! @file window.hpp
//! @brief Defines the generic Window base class for SendWindow and RecvWindow.
//!
//! @author Peter Georg
#ifndef pMR_WINDOW_H
#define pMR_WINDOW_H

#include <vector>
#include <stdexcept>
#include <limits>
#include <cstdint>
#include <ios>
#include <sstream>
#include "config.hpp"
#include "misc/allocator.hpp"
#include "misc/print.hpp"
#include "misc/profiling.hpp"

namespace pMR
{
    class Connection;

    //! @brief Generic base class for Windows.
    template<typename T>
    class Window
    {
        public:
            Window(T *buffer, std::uint32_t const count);
            explicit Window(std::uint32_t const count);
            Window(const Window&) = delete;
            Window(Window&&) = default;
            Window& operator=(const Window&) = delete;
            Window& operator=(Window&&) = default;
            ~Window() = default;
            //! @brief Direct access specified element of the internal buffer.
            //! @param index Position of element in internal buffer.
            //! @return Reference to element at position index.
            T& operator[](int const index);
            //! @brief Direct access specified element of the internal buffer.
            //! @param index Position of element in internal buffer.
            //! @return Reference to const to element at position index.
            T const& operator[](int const index) const;
            //! @brief Direct access specified element of the internal buffer
            //!     with bounds checking.
            //! @param index Position of element in internal buffer.
            //! @return Reference to element at position index.
            T& at(int const index);
            //! @brief Direct access specified element of the internal buffer
            //!     with bounds checking.
            //! @param index Position of element in internal buffer.
            //! @return Reference to const to element at position index.
            T const& at(int const index) const;
            //! @brief Returns an iterator to the beginning of the internal
            //!     buffer.
            //! @return Iterator to the first element of the internal buffer.
            T* begin();
            //! @brief Returns an iterator to const to the beginning of the
            //!     internal buffer.
            //! @return Iterator to const to the first element of the internal
            //!     buffer.
            T const* begin() const;
            //! @brief Returns an iterator to const to the beginning of the
            //!     internal buffer.
            //! @return Iterator to const to the first element of the internal
            //!     buffer.
            T const* cbegin() const;
            //! @brief Returns an Iterator to the end of the internal buffer.
            //! @return Iterator to the element following the last element of
            //!     the internal buffer.
            T* end();
            //! @brief Returns an Iterator to to const the end of the internal
            //!     buffer.
            //! @return Iterator to const to the element following the last
            //!     element of the internal buffer.
            T const* end() const;
            //! @brief Returns an Iterator to const to the end of the internal
            //!     buffer.
            //! @return Iterator to const to the element following the last
            //!     element of the internal buffer.
            T const* cend() const;
            //! @brief Returns the number of elements in the internal buffer.
            //! @return The number of elements in the internal buffer.
            std::uint32_t size() const;
            //! @brief Compare current Window with specified input data.
            //! @details Checks whether the current Window is the same as a
            //!     Window with the specified buffer and count.
            //! @param buffer Pointer to contigious send/receive buffer.
            //! @param count Number of elements of type T in buffer.
            //! @return true if the Window is the same, false otherwise.
            bool isSame(T *const buffer, std::uint32_t const count);
            //! @brief Compare current Window with specified input data.
            //! @details Checks whether the current Window is suitable for the
            //!     specified count using internal buffering.
            //! @param count Number of elements of type T in buffer.
            //! @return true if the Window is suitable, false otherwise.
            bool isSame(std::uint32_t const count);
        protected:
            std::vector<T, AlignedAllocator<T>> mVector;
            T* mBuffer;
            std::uint32_t mCount;
            void checkBufferType();
            void checkBufferSize(std::uint32_t const count);
            void checkBoundaries(std::uint32_t const offset,
                    std::uint32_t const count);
#ifdef PROFILING
            std::uint32_t mSizeByte = 0;
            std::uint64_t mIterations = 0;
            double mTimeInit = 0.0;
            double mTimePost = 0.0;
            double mTimeWait = 0.0;
            double mTimeCopy = 0.0;
            template<std::size_t N>
            void resetStats(char const (&type)[N]);
            template<std::size_t N>
            void printStats(char const (&type)[N]);
#endif // PROFILING
    };
}

template<typename T>
pMR::Window<T>::Window(T *buffer, std::uint32_t const count)
    :   mBuffer(buffer), mCount(count)
{
#ifdef PROFILING
    mSizeByte = count * sizeof(T);
#endif // PROFILING
    checkBufferType();
    checkBufferSize(mCount);
}

template<typename T>
pMR::Window<T>::Window(std::uint32_t const count)
    :   mVector(count), mBuffer(mVector.data()), mCount(count)
{
#ifdef PROFILING
    mSizeByte = count * sizeof(T);
#endif // PROFILING
    checkBufferType();
    checkBufferSize(mCount);
}

template<typename T>
T& pMR::Window<T>::operator[](int const index)
{
    return mVector[index];
}

template<typename T>
T const& pMR::Window<T>::operator[](int const index) const
{
    return mVector[index];
}

template<typename T>
T& pMR::Window<T>::at(int const index)
{
    return mVector.at(index);
}

template<typename T>
T const& pMR::Window<T>::at(int const index) const
{
    return mVector.at(index);
}

template<typename T>
T* pMR::Window<T>::begin()
{
    return mVector.begin();
}

template<typename T>
T const* pMR::Window<T>::begin() const
{
    return mVector.begin();
}

template<typename T>
T const* pMR::Window<T>::cbegin() const
{
    return mVector.cbegin();
}

template<typename T>
T* pMR::Window<T>::end()
{
    return mVector.end();
}

template<typename T>
T const* pMR::Window<T>::end() const
{
    return mVector.end();
}

template<typename T>
T const* pMR::Window<T>::cend() const
{
    return mVector.end();
}

template<typename T>
std::uint32_t pMR::Window<T>::size() const
{
    return mVector.size();
}

template<typename T>
bool pMR::Window<T>::isSame(T *const buffer, std::uint32_t const count)
{
    if(mCount != count)
    {
        return false;
    }
    if(mBuffer != buffer)
    {
        return false;
    }
    return true;
}

template<typename T>
bool pMR::Window<T>::isSame(std::uint32_t const count)
{
    if(mCount != count)
    {
        return false;
    }
    if(mBuffer != mVector.data())
    {
        return false;
    }
    if(mCount != mVector.size())
    {
        return false;
    }
    return true;
}

template<typename T>
void pMR::Window<T>::checkBufferType()
{
    static_assert(std::is_trivial<T>::value,
            "Window template type is not trivial.");
}

template<typename T>
void pMR::Window<T>::checkBufferSize(std::uint32_t const count)
{
    if(count * sizeof(T) > std::numeric_limits<std::uint32_t>::max())
    {
        throw std::overflow_error("Message Size");
    }

#ifdef HINT
    if(count == 0)
    {
        print("pMR: HINT: Using zero size Window. Intentional?");
    }
#endif // HINT
}

template<typename T>
void pMR::Window<T>::checkBoundaries(std::uint32_t const offset,
        std::uint32_t const count)
{
    if(offset + count > mVector.size())
    {
        throw std::out_of_range("Window buffer");
    }
}

#ifdef PROFILING
template<typename T>
template<std::size_t N>
void pMR::Window<T>::resetStats(char const (&type)[N])
{
    printStats(type);
    mSizeByte = mCount * sizeof(T);
    mIterations = 0;
    mTimeInit = 0.0;
    mTimePost = 0.0;
    mTimeWait = 0.0;
    mTimeCopy = 0.0;
}

template<typename T>
template<std::size_t N>
void pMR::Window<T>::printStats(char const (&type)[N])
{
    if(mIterations)
    {
        print("pMR:", type, "SizeByte", mSizeByte, "Init", mTimeInit,
                "Post", mTimePost, "Wait", mTimeWait, "Sum",
                mTimeInit + mTimePost + mTimeWait,
                "Copy", mTimeCopy, "Iterations", mIterations);
    }
}
#endif // PROFILING
#endif // pMR_WINDOW_H
