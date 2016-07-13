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

//! @file sendwindow.hpp
//! @brief Public interface for SendWindow.
//!
//! @author Peter Georg
#ifndef pMR_SENDWINDOW_H
#define pMR_SENDWINDOW_H

#include <cstdint>
#include "config.hpp"
#include "sendmemorywindow.hpp"
#include "window.hpp"
#include "misc/thread.hpp"
#include "misc/numeric.hpp"
#include "misc/type.hpp"

namespace pMR
{
    //! @brief Send Window.
    template<typename T>
    class SendWindow : public Window<T>
    {
        public:
            //! @brief Window for data to send to target.
            //! @details Creates a SendWindow for specified connection. Sends
            //!     count elements T, stored in contigious memory region
            //!     buffer points to, to target.
            //! @param connection Connection to associate Window with.
            //! @param buffer Pointer to memory region used as send buffer.
            //! @param count Number of elements of type T in send buffer.
            SendWindow(Connection const &connection,
                    T *buffer, std::uint32_t const count);
            //! @brief Window for data to send to target.
            //! @details Creates a SendWindow for specified connection. Sends
            //!     count elements T, stored in internal buffer, to target.
            //! @param connection Connection to associate Window with.
            //! @param count Number of elements of type T in send buffer.
            SendWindow(Connection const &connection, std::uint32_t const count);
            SendWindow(const SendWindow&) = delete;
            SendWindow(SendWindow&&) = default;
            SendWindow& operator=(const SendWindow&) = delete;
            SendWindow& operator=(SendWindow&&) = default;
            ~SendWindow();

            //! @brief Initialize a send routine.
            //! @note Send buffer associated with this send window may still
            //!     be accessed - read and write.
            //! @warning Any outstanding synchronization or previously
            //!     initiated send routine, associated with the same connection,
            //!     has to be finished before initializing a send routine.
            //! @note Receive and send routines associated with the same
            //!     connection may be initialized simultaneously.
            void init();
            //! @brief Post previously initialized send routine.
            //! @warning Send buffer associated with this send window may be
            //!     accessed read only as soon as send routine has been posted.
            //! @note Depending on the used provider, this routine might be
            //!     blocking.
            void post();
            //! @brief Wait for previously posted send routine to finish.
            //! @note Write access to send buffer is allowed again after wait.
            //! @note Blocking routine.
            void wait();

            //! @brief Copy data to internal buffer.
            //! @details Copies data from storage - inputIt iterator points to -
            //!     to internal buffer starting at offset with bounds checking. 
            //! @param inputIt Iterator to source.
            //! @param offset Offset for the internal buffer.
            //! @param count Number of elements to copy.
            template<class Iterator>
            void insert(Iterator inputIt,
                    std::uint32_t const offset, std::uint32_t const count);
            //! @brief Copy data to internal buffer.
            //! @details Fills the internal buffer - starting at the beginning -
            //!     with data pointed to by inputIt.
            //! @warning The container inputIt points to must be at least of the
            //!     same size as the internal buffer.
            //! @param inputIt Iterator to source.
            template<class Iterator>
            void insert(Iterator inputIt);
            //! @brief Multithreaded copy data to internal buffer.
            //! @details Copies data from storage - inputIt iterator points to -
            //!     to internal buffer starting at offset with bounds checking. 
            //!     The copy operation is shared among all threadCount threads
            //!     calling the function.
            //! @warning threadID has to be in range [0, threadCount).
            //! @warning The function has to be called with every threadID in
            //!     the range [0, threadCount).
            //! @warning inputIt must be of type random access iterator.
            //! @param inputIt Iterator to source.
            //! @param offset Offset for the internal buffer.
            //! @param count Number of elements to copy.
            //! @param threadID ID of the current Thread.
            //! @param threadCount Number of threads calling the function.
            template<class Iterator>
            void insertMT(Iterator inputIt,
                   std::uint32_t const offset, std::uint32_t const count,
                   int const threadID, int const threadCount);
            //! @brief Multithreaded copy data to internal buffer.
            //! @details Fills the internal buffer - starting at the beginning -
            //!     with data pointed to by inputIt. The copy operation is
            //!     shared among all threadCount threads calling the function.
            //! @warning The container inputIt points to must be at least of the
            //!     same size as the internal buffer.
            //! @warning threadID has to be in range [0, threadCount).
            //! @warning The function has to be called with every threadID in
            //!     the range [0, threadCount).
            //! @warning inputIt must be of type random access iterator.
            //! @param inputIt Iterator to source.
            //! @param threadID ID of the current Thread.
            //! @param threadCount Number of threads calling the function.
            template<class Iterator>
            void insertMT(Iterator inputIt,
                    int const threadID, int const threadCount);
        private:
            SendMemoryWindow mMemoryWindow;
    };
}

template<typename T>
pMR::SendWindow<T>::SendWindow(Connection const &connection,
        T *const buffer, std::uint32_t const count)
    :   Window<T>(buffer, count),
        mMemoryWindow(connection, buffer, count * sizeof(T)) { }

template<typename T>
pMR::SendWindow<T>::SendWindow(Connection const &connection,
        std::uint32_t const count)
    :   Window<T>(count),
        mMemoryWindow(connection, this->mBuffer, count * sizeof(T)) { }

template<typename T>
pMR::SendWindow<T>::~SendWindow()
{
#ifdef PROFILING
    this->printStats("Send");
#endif // PROFILING
}

template<typename T>
void pMR::SendWindow<T>::init()
{
    PROF_START(this->mTimeInit);
    mMemoryWindow.init();
    PROF_STOP(this->mTimeInit);
}

template<typename T>
void pMR::SendWindow<T>::post()
{
    PROF_START(this->mTimePost);
    mMemoryWindow.post();
    PROF_STOP(this->mTimePost);
}

template<typename T>
void pMR::SendWindow<T>::wait()
{
    PROF_START(this->mTimeWait);
    mMemoryWindow.wait();
    PROF_STOP(this->mTimeWait);
    PROF_COUNT(this->mIterations);
}

template<typename T>
template<class Iterator>
void pMR::SendWindow<T>::insert(Iterator inputIt,
        std::uint32_t const offset, std::uint32_t const count)
{
    PROF_START(this->mTimeCopy);
    this->checkBoundaries(offset, count);
    std::copy_n(inputIt, count, this->mVector.begin() + offset);
    PROF_STOP(this->mTimeCopy);
}

template<typename T>
template<class Iterator>
void pMR::SendWindow<T>::insert(Iterator inputIt)
{
    return insert(inputIt, 0, this->mVector.size());
}

template<typename T>
template<class Iterator>
void pMR::SendWindow<T>::insertMT(Iterator inputIt,
        std::uint32_t const offset, std::uint32_t const count,
        int const threadID, int const threadCount)
{
    PROF_START_THREAD(this->mTimeCopy);
    this->checkBoundaries(offset, count);
    static_assert(isRandomAccessIterator<Iterator>(),
            "Iterator is not of random access type");

    std::uint32_t threadStart;
    std::uint32_t threadEnd;
    splitWorkToThreads(offset, offset + count, threadID, threadCount,
            threadStart, threadEnd,
            getLeastCommonMultiple(static_cast<std::uint32_t>(alignment),
                static_cast<std::uint32_t>(sizeof(T))));

    std::copy_n(inputIt + threadStart, threadEnd - threadStart,
            this->mVector.begin() + threadStart);
    PROF_STOP_THREAD(this->mTimeCopy);
}

template<typename T>
template<class Iterator>
void pMR::SendWindow<T>::insertMT(Iterator inputIt,
        int const threadID, int const threadCount)
{
    return insertMT(inputIt, 0, this->mVector.size(), threadID, threadCount);
}
#endif // pMR_SENDWINDOW_H
