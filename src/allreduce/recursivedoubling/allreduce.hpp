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

#ifndef pMR_ALLREDUCE_RECURSIVEDOUBLING_ALLREDUCE_H
#define pMR_ALLREDUCE_RECURSIVEDOUBLING_ALLREDUCE_H

#include <cstdint>
#include <algorithm>

#include "communicator.hpp"
#include "connection.hpp"
#include "sendmemorywindow.hpp"
#include "recvmemorywindow.hpp"
#include "misc/allocator.hpp"

namespace pMR
{
    class AllReduce
    {
        public:
            AllReduce(Communicator const communicator,
                    std::uint32_t const sizeByte);
            AllReduce(const AllReduce&) = delete;
            AllReduce(AllReduce&&) = delete;
            AllReduce& operator=(const AllReduce&) = delete;
            AllReduce& operator=(AllReduce&&) = delete;
            ~AllReduce() = default;

            template<typename T, class Iterator>
            void insert(Iterator inputIt, std::uint32_t const count);

            template<typename T, class Iterator>
            void extract(Iterator outputIt, std::uint32_t const count);

            template<typename T>
            T* data();
            template<typename T>
            T const* data() const;
            template<typename T>
            std::uint32_t size() const;

            template<typename T>
            void execute(void (*reduce)
                    (T const *in, T *inout, std::uint32_t const count),
                    std::uint32_t const count);
        private:
            std::uint32_t const mSizeByte;
            // Connection for (potential) pre and post step
            std::unique_ptr<pMR::Connection> mPPConnection;
            // Connections for recursive-doubling
            std::vector<pMR::Connection> mRDConnections;
            // Memory Buffer(s)
            std::vector<unsigned char, pMR::Allocator<unsigned char>>
                mBuffers;
            // Memory Windows for pre and post step
            std::unique_ptr<pMR::SendMemoryWindow> mPPSendWindow;
            std::unique_ptr<pMR::RecvMemoryWindow> mPPRecvWindow;
            // Memory Windows for recursive-doubling
            std::vector<pMR::SendMemoryWindow> mRDSendWindows;
            std::vector<pMR::RecvMemoryWindow> mRDRecvWindows;

            template<typename T>
            void checkBoundaries(std::uint32_t const count);
    };
}

template<typename T, class Iterator>
void pMR::AllReduce::insert(Iterator inputIt, std::uint32_t const count)
{
    checkBoundaries<T>(count);
    std::copy_n(inputIt, count, reinterpret_cast<T*>(mBuffers.data()));
}

template<typename T, class Iterator>
void pMR::AllReduce::extract(Iterator outputIt, std::uint32_t const count)
{
    checkBoundaries<T>(count);
    std::copy_n(reinterpret_cast<T const*>(mBuffers.data()), count, outputIt);
}

template<typename T>
T* pMR::AllReduce::data()
{
    return reinterpret_cast<T*>(mBuffers.data());
}

template<typename T>
T const* pMR::AllReduce::data() const
{
    return reinterpret_cast<T const*>(mBuffers.data());
}

template<typename T>
std::uint32_t pMR::AllReduce::size() const
{
    return mSizeByte / sizeof(T);
}

template<typename T>
void pMR::AllReduce::execute(
        void (*reduce) (T const *in, T *inout, std::uint32_t const count),
        std::uint32_t const count)
{
    // Init data exchange for all connections.
    // This way we can at least overlap the control messages thanks to
    // using a different receive buffer for each connection.
    if(mPPConnection)
    {
        if(mRDConnections.size() == 0)
        {
            mPPSendWindow->init();
        }
        else
        {
            mPPRecvWindow->init();
        }
    }

    decltype(mRDSendWindows.begin()) sendWindow;
    decltype(mRDRecvWindows.begin()) recvWindow;
    for(sendWindow = mRDSendWindows.begin(),
            recvWindow = mRDRecvWindows.begin();
            sendWindow != mRDSendWindows.end() &&
            recvWindow != mRDRecvWindows.cend();
            ++sendWindow, ++recvWindow)
    {
        recvWindow->init();
        sendWindow->init();
    }

    // Pre step
    if(mPPConnection)
    {
        if(mRDConnections.size() == 0)
        {
            mPPSendWindow->post();
            mPPSendWindow->wait();
        }
        else
        {
            mPPRecvWindow->post();
            mPPRecvWindow->wait();
            reduce(reinterpret_cast<T const*>(mPPRecvWindow->data()),
                   reinterpret_cast<T*>(mPPSendWindow->data()), count);
        }
    }

    // Recursive-doubling
    for(sendWindow = mRDSendWindows.begin(),
            recvWindow = mRDRecvWindows.begin();
            sendWindow != mRDSendWindows.end() &&
            recvWindow != mRDRecvWindows.cend();
            ++sendWindow, ++recvWindow)
    {
        sendWindow->post();
        recvWindow->post();
        sendWindow->wait();
        recvWindow->wait();
        reduce(reinterpret_cast<T const*>(recvWindow->data()),
               reinterpret_cast<T*>(sendWindow->data()), count);
    }

    // Post step
    if(mPPConnection)
    {
        if(mRDConnections.size() == 0)
        {
            mPPRecvWindow->init();
            mPPRecvWindow->post();
            mPPRecvWindow->wait();
        }
        else
        {
            mPPSendWindow->init();
            mPPSendWindow->post();
            mPPSendWindow->wait();
        }
    }
}

template<typename T>
void pMR::AllReduce::checkBoundaries(std::uint32_t const count)
{
    if(count > size<T>())
    {
        throw std::out_of_range("AllReduce buffer");
    }
}
#endif // pMR_ALLREDUCE_RECURSIVEDOUBLING_ALLREDUCE_H
