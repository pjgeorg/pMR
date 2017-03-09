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

//! @file recvwindow.hpp
//! @brief Public interface for RecvWindow.
//!
//! @author Peter Georg
#ifndef pMR_RECVWINDOW_H
#define pMR_RECVWINDOW_H

#include <cstdint>
#include "misc/numeric.hpp"
#include "misc/thread.hpp"
#include "misc/type.hpp"
#include "config.hpp"
#include "recvmemorywindow.hpp"
#include "window.hpp"

namespace pMR
{
    //! @brief Receive Window.
    template<typename T>
    class RecvWindow : public Window<T>
    {
    public:
        //! @brief Window to store data received from target.
        //! @details Creates a RecvWindow for specified connection. Uses the
        //!     contigious memory region - which can fit count elements of
        //!     type T - buffer points to, to store data received from
        //!     target.
        //! @param connection Connection to associate Window with.
        //! @param buffer Pointer to memory region used as receive buffer.
        //! @param count Number of elements of type T in receive buffer.
        RecvWindow(
            Connection const &connection, T *buffer, size_type const count);
        //! @brief Window to store data received from target.
        //! @details Creates a RecvWindow for specified connection. Uses an
        //!     internal buffer - which can fit count elements of type T -
        //!     to store data received from target.
        //! @param connection Connection to associate Window with.
        //! @param count Number of elements of type T in receive buffer.
        RecvWindow(Connection const &connection, size_type const count);
        RecvWindow(RecvWindow const &) = delete;
        RecvWindow(RecvWindow &&) = default;
        RecvWindow &operator=(RecvWindow const &) = delete;
        RecvWindow &operator=(RecvWindow &&) = default;
        ~RecvWindow();

        //! @brief Initialize a receive routine.
        //! @warning Receive buffer associated with this receive window may
        //!     not be accessed - read and write - as soon as the receive
        //!     routine has been initialized.
        //! @warning Any outstanding synchronization or previously
        //!     initiated receive routine, associated with the same
        //!     connection, has to be finished before initializing a
        //!     receive routine.
        //! @note Send and receive routines associated with the same
        //!     connection may be initialized simultaneously.
        void init();
        //! @brief Post previously initialized receive routine.
        //! @note Depending on the used provider, this routine might be
        //!     blocking.
        void post();
        //! @brief Wait for previously posted receive routine to finish.
        //! @note Access to receive buffer is allowed again after wait.
        //! @note Blocking routine.
        void wait();

        //! @brief Copy data from internal buffer.
        //! @details Copies data from the internal buffer - starting at the
        //!     specified offset - to the location specified by outputIt
        //!     with bounds checking.
        //! @param outputIt Iterator to destination.
        //! @param offset Offset for the internal buffer.
        //! @param count Number of elements to copy.
        template<class Iterator>
        void extract(
            Iterator outputIt, size_type const offset, size_type const count);
        //! @brief Copy data from internal buffer.
        //! @details Copies all elements - starting at the beginning - of
        //!     the internal buffer to destination specified by outputIt.
        //! @warning The container outputIt points to must be at least of
        //!     the same size as the internal buffer.
        //! @param outputIt Iterator to destination.
        template<class Iterator>
        void extract(Iterator outputIt);
        //! @brief Multithreaded copy data from internal buffer.
        //! @details Copies data from the internal buffer - starting at the
        //!     specified offset - to the location specified by outputIt
        //!     with bounds checking. The copy operation is shared among
        //!     all threadCount threads calling the function.
        //! @warning threadID has to be in range [0, threadCount).
        //! @warning The function has to be called with every threadID in
        //!     the range [0, threadCount).
        //! @warning outputIt must be of type random access iterator.
        //! @param outputIt Iterator to destination.
        //! @param offset Offset for the internal buffer.
        //! @param count Number of elements to copy.
        //! @param threadID ID of the current Thread.
        //! @param threadCount Number of threads calling the function.
        template<class Iterator>
        void extractMT(Iterator outputIt, size_type const offset,
            size_type const count, int const threadID, int const threadCount);
        //! @brief Multithreaded copy data from internal buffer.
        //! @details Copies all elements - starting at the beginning - of
        //!     the internal buffer to destination specified by outputIt.
        //!     The copy operation is shared among all threadCount threads
        //!     calling the function.
        //! @warning The container outputIt points to must be at least of
        //!     the same size as the internal buffer.
        //! @warning threadID has to be in range [0, threadCount).
        //! @warning The function has to be called with every threadID in
        //!     the range [0, threadCount).
        //! @warning outputIt must be of type random access iterator.
        //! @param outputIt Iterator to data to be copied.
        //! @param threadID ID of the current Thread.
        //! @param threadCount Number of threads calling the function.
        template<class Iterator>
        void extractMT(
            Iterator outputIt, int const threadID, int const threadCount);

    private:
        RecvMemoryWindow mMemoryWindow;
    };
}

template<typename T>
pMR::RecvWindow<T>::RecvWindow(
    Connection const &connection, T *const buffer, size_type const count)
    : Window<T>(buffer, count)
    , mMemoryWindow(
          connection, buffer, {static_cast<size_type>(count * sizeof(T))})
{
}

template<typename T>
pMR::RecvWindow<T>::RecvWindow(
    Connection const &connection, size_type const count)
    : Window<T>(count)
    , mMemoryWindow(connection, this->mBuffer,
          {static_cast<size_type>(count * sizeof(T))})
{
}

template<typename T>
pMR::RecvWindow<T>::~RecvWindow()
{
#ifdef pMR_PROFILING
    this->printStats("Recv");
#endif // pMR_PROFILING
}

template<typename T>
void pMR::RecvWindow<T>::init()
{
    pMR_PROF_START(this->mTimeInit);
    mMemoryWindow.init();
    pMR_PROF_STOP(this->mTimeInit);
}

template<typename T>
void pMR::RecvWindow<T>::post()
{
    pMR_PROF_START(this->mTimePost);
    mMemoryWindow.post();
    pMR_PROF_STOP(this->mTimePost);
}

template<typename T>
void pMR::RecvWindow<T>::wait()
{
    pMR_PROF_START(this->mTimeWait);
    mMemoryWindow.wait();
    pMR_PROF_STOP(this->mTimeWait);
    pMR_PROF_COUNT(this->mIterations);
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extract(
    Iterator outputIt, size_type const offset, size_type const count)
{
    pMR_PROF_START(this->mTimeCopy);
    this->checkBoundaries({offset}, {count});
    std::copy_n(this->mVector.cbegin() + offset, count, outputIt);
    pMR_PROF_STOP(this->mTimeCopy);
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extract(Iterator outputIt)
{
    return extract(outputIt, 0, {static_cast<size_type>(this->mVector.size())});
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extractMT(Iterator outputIt, size_type const offset,
    size_type const count, int const threadID, int const threadCount)
{
    pMR_PROF_START_THREAD(this->mTimeCopy);
    this->checkBoundaries({offset}, {count});
    static_assert(isRandomAccessIterator<Iterator>(),
        "Iterator is not of random access type");

    size_type threadStart;
    size_type threadEnd;
    splitWorkToThreads(offset, static_cast<size_type>(offset + count), threadID,
        threadCount, threadStart, threadEnd,
        getLeastCommonMultiple(static_cast<size_type>(cAlignment),
            static_cast<size_type>(sizeof(T))));

    std::copy_n(this->mVector.cbegin() + threadStart, threadEnd - threadStart,
        outputIt + threadStart);
    pMR_PROF_STOP_THREAD(this->mTimeCopy);
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extractMT(
    Iterator outputIt, int const threadID, int const threadCount)
{
    return extractMT(
        outputIt, 0, {this->mVector.size()}, {threadID}, {threadCount});
}
#endif // pMR_RECVWINDOW_H
