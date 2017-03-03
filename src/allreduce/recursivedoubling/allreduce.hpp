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

#ifndef pMR_ALLREDUCE_RECURSIVE_DOUBLING_ALLREDUCE_H
#define pMR_ALLREDUCE_RECURSIVE_DOUBLING_ALLREDUCE_H

#include "../../communicator.hpp"
#include "../../connection.hpp"
#include "../../misc/allocator.hpp"
#include "../../operations.hpp"
#include "../../recvmemorywindow.hpp"
#include "../../sendmemorywindow.hpp"

namespace pMR
{
    namespace RecursiveDoubling
    {
        class AllReduce
        {
        public:
            AllReduce(Communicator const &communicator, void *buffer,
                size_type const sizeByte);
            AllReduce(const AllReduce &) = delete;
            AllReduce(AllReduce &&) = delete;
            AllReduce &operator=(const AllReduce &) = delete;
            AllReduce &operator=(AllReduce &&) = delete;
            ~AllReduce() = default;

            template<typename T>
            bool execute(Operation reduce, size_type const count);

            template<typename T>
            bool execute(
                void (*reduce)(T const *in, T *inout, size_type const count),
                size_type const count);

        private:
            void *mBuffer;
            size_type const mSizeByte;
            // Connection for (potential) pre and post step
            std::unique_ptr<pMR::Connection> mPPConnection;
            // Connections for recursive-doubling
            std::vector<pMR::Connection> mRDConnections;
            // Memory Buffer(s)
            std::vector<unsigned char, pMR::Allocator<unsigned char>> mBuffers;
            // Memory Windows for pre and post step
            std::unique_ptr<pMR::SendMemoryWindow> mPPSendWindow;
            std::unique_ptr<pMR::RecvMemoryWindow> mPPRecvWindow;
            // Memory Windows for recursive-doubling
            std::vector<pMR::SendMemoryWindow> mRDSendWindows;
            std::vector<pMR::RecvMemoryWindow> mRDRecvWindows;
        };
    }
}

template<typename T>
bool pMR::RecursiveDoubling::AllReduce::execute(
    Operation op, size_type const count)
{
    switch(op)
    {
        case Operation::Max:
            return {execute<T>(&max, count)};
            break;
        case Operation::Min:
            return {execute<T>(&min, count)};
            break;
        case Operation::Sum:
            return {execute<T>(&sum, count)};
            break;
        case Operation::Prod:
            return {execute<T>(&sum, count)};
            break;
        default:
            return {false};
            break;
    }
}

template<typename T>
bool pMR::RecursiveDoubling::AllReduce::execute(
    void (*reduce)(T const *in, T *inout, size_type const count),
    size_type const count)
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
            reduce(reinterpret_cast<T const *>(mPPRecvWindow->data()),
                reinterpret_cast<T *>(mPPSendWindow->data()), count);
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
        reduce(reinterpret_cast<T const *>(recvWindow->data()),
            reinterpret_cast<T *>(sendWindow->data()), count);
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

    return true;
}
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING_ALLREDUCE_H
