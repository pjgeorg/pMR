//! @file recvwindow.hpp
//! @brief Public interface for RecvWindow.
//!
//! @author Peter Georg
#ifndef pMR_RECVWINDOW_H
#define pMR_RECVWINDOW_H

#include <cstdint>
#include "config.hpp"
#include "recvmemorywindow.hpp"
#include "window.hpp"
#include "misc/thread.hpp"
#include "misc/numeric.hpp"
#include "misc/type.hpp"

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
            RecvWindow(Connection const &connection,
                    T *buffer, std::uint32_t const count);
            //! @brief Window to store data received from target.
            //! @details Creates a RecvWindow for specified connection. Uses an
            //!     internal buffer - which can fit count elements of type T -
            //!     to store data received from target.
            //! @param connection Connection to associate Window with.
            //! @param count Number of elements of type T in receive buffer.
            RecvWindow(Connection const &connection, std::uint32_t const count);
            RecvWindow(const RecvWindow&) = delete;
            RecvWindow(RecvWindow&&) = delete;
            RecvWindow& operator=(const RecvWindow&) = delete;
            RecvWindow& operator=(RecvWindow &&) = delete;
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
            void extract(Iterator outputIt,
                    std::uint32_t const offset, std::uint32_t const count);
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
            void extractMT(Iterator outputIt,
                   std::uint32_t const offset, std::uint32_t const count,
                   int const threadID, int const threadCount);
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
            void extractMT(Iterator outputIt,
                    int const threadID, int const threadCount);
        private:
            RecvMemoryWindow mMemoryWindow;
    };
}

template<typename T>
pMR::RecvWindow<T>::RecvWindow(Connection const &connection,
        T *const buffer, std::uint32_t const count)
    :   Window<T>(buffer, count),
        mMemoryWindow(connection, buffer, count * sizeof(T)) { }

template<typename T>
pMR::RecvWindow<T>::RecvWindow(Connection const &connection,
        std::uint32_t const count)
    :   Window<T>(count),
        mMemoryWindow(connection, this->mBuffer, count * sizeof(T)) { }

template<typename T>
pMR::RecvWindow<T>::~RecvWindow()
{
#ifdef PROFILING
    this->printStats("Recv");
#endif // PROFILING
}

template<typename T>
void pMR::RecvWindow<T>::init()
{
    PROF_START(this->mTimeInit);
    mMemoryWindow.init();
    PROF_STOP(this->mTimeInit);
}

template<typename T>
void pMR::RecvWindow<T>::post()
{
    PROF_START(this->mTimePost);
    mMemoryWindow.post();
    PROF_STOP(this->mTimePost);
}

template<typename T>
void pMR::RecvWindow<T>::wait()
{
    PROF_START(this->mTimeWait);
    mMemoryWindow.wait();
    PROF_STOP(this->mTimeWait);
    PROF_COUNT(this->mIterations);
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extract(Iterator outputIt,
        std::uint32_t const offset, std::uint32_t const count)
{
    PROF_START(this->mTimeCopy);
    this->checkBoundaries(offset, count);
    std::copy_n(this->mVector.cbegin() + offset, count, outputIt);
    PROF_STOP(this->mTimeCopy);
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extract(Iterator outputIt)
{
    return extract(outputIt, 0, this->mVector.size());
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extractMT(Iterator outputIt,
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

    std::copy_n(this->mVector.cbegin() + threadStart, threadEnd - threadStart,
            outputIt + threadStart);
    PROF_STOP_THREAD(this->mTimeCopy);
}

template<typename T>
template<class Iterator>
void pMR::RecvWindow<T>::extractMT(Iterator outputIt,
        int const threadID, int const threadCount)
{
    return extractMT(outputIt, 0, this->mVector.size(), threadID, threadCount);
}
#endif // pMR_RECVWINDOW_H
