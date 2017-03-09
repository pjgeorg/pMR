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

#include <cstring>
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
            bool execute(
                void *buffer, Operation const op, size_type const count);

            template<typename T>
            bool execute(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count);

            template<typename T>
            bool executeBit(
                void *buffer, Operation const op, size_type const count);

            template<typename T>
            bool executeBit(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count);

        private:
            // Domain reduction
            bool mDomainRoot = false;
            std::vector<pMR::Connection> mDomainConnections;
            std::vector<unsigned char, pMR::Allocator<unsigned char>>
                mDomainBuffers;
            std::vector<pMR::SendMemoryWindow> mDomainSendWindows;
            std::vector<pMR::RecvMemoryWindow> mDomainRecvWindows;
            // Pre Recursive-doubling reduction
            std::unique_ptr<pMR::Connection> mPreConnection;
            std::vector<unsigned char, pMR::Allocator<unsigned char>>
                mPreBuffer;
            std::unique_ptr<pMR::SendMemoryWindow> mPreSendWindow;
            std::unique_ptr<pMR::RecvMemoryWindow> mPreRecvWindow;
            // Recursive-doubling
            int mRDPartition = -1;
            std::vector<pMR::Connection> mRDConnections;
            std::vector<unsigned char, pMR::Allocator<unsigned char>>
                mRDBuffers;
            std::vector<pMR::SendMemoryWindow> mRDSendWindows;
            std::vector<pMR::RecvMemoryWindow> mRDRecvWindows;

            void initWindows();
            template<typename T>
            void domainReduce(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count);
            template<typename T>
            void preReduce(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count);
            template<typename T>
            void recursiveDoubling(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count);
            template<typename T>
            void reduceBroadcast(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count);
            void postBroadcast();
            void domainBroadcast();
        };
    }
}

template<typename T>
bool pMR::RecursiveDoubling::AllReduce::execute(
    void *buffer, Operation const op, size_type const count)
{
    switch(op)
    {
        case Operation::Max:
            return {execute<T>(buffer, &max, count)};
            break;
        case Operation::Min:
            return {execute<T>(buffer, &min, count)};
            break;
        case Operation::Sum:
            return {execute<T>(buffer, &sum, count)};
            break;
        case Operation::Prod:
            return {execute<T>(buffer, &prod, count)};
            break;
        default:
            return {false};
            break;
    }
}

template<typename T>
bool pMR::RecursiveDoubling::AllReduce::execute(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
    initWindows();
    domainReduce(buffer, op, count);
    preReduce(buffer, op, count);

    recursiveDoubling(buffer, op, count);

    postBroadcast();
    domainBroadcast();

    return {true};
}

template<typename T>
bool pMR::RecursiveDoubling::AllReduce::executeBit(
    void *buffer, Operation const op, size_type const count)
{
    switch(op)
    {
        case Operation::Max:
            return {executeBit<T>(buffer, &max, count)};
            break;
        case Operation::Min:
            return {executeBit<T>(buffer, &min, count)};
            break;
        case Operation::Sum:
            return {executeBit<T>(buffer, &sum, count)};
            break;
        case Operation::Prod:
            return {executeBit<T>(buffer, &prod, count)};
            break;
        default:
            return {false};
            break;
    }
}

template<typename T>
bool pMR::RecursiveDoubling::AllReduce::executeBit(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
    initWindows();
    domainReduce(buffer, op, count);
    preReduce(buffer, op, count);

    reduceBroadcast(buffer, op, count);

    postBroadcast();
    domainBroadcast();

    return {true};
}

template<typename T>
void pMR::RecursiveDoubling::AllReduce::domainReduce(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
    if(mDomainRoot)
    {
        for(auto &window : mDomainRecvWindows)
        {
            window.post();
            window.wait();
            op(reinterpret_cast<T const *>(window.data()),
                reinterpret_cast<T *>(buffer), count);
        }
    }
    else
    {
        for(auto &window : mDomainSendWindows)
        {
            window.post();
            window.wait();
        }
    }
}

template<typename T>
void pMR::RecursiveDoubling::AllReduce::preReduce(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
    if(mPreConnection)
    {
        if(mRDConnections.size() == 0)
        {
            mPreSendWindow->post();
            mPreSendWindow->wait();
        }
        else
        {
            mPreRecvWindow->post();
            mPreRecvWindow->wait();
            op(reinterpret_cast<T const *>(mPreRecvWindow->data()),
                reinterpret_cast<T *>(buffer), count);
        }
    }
}

template<typename T>
void pMR::RecursiveDoubling::AllReduce::recursiveDoubling(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
    auto sendWindow = mRDSendWindows.begin();
    auto recvWindow = mRDRecvWindows.begin();
    while(sendWindow != mRDSendWindows.end() &&
        recvWindow != mRDRecvWindows.end())
    {
        sendWindow->post();
        recvWindow->post();
        sendWindow->wait();
        recvWindow->wait();
        op(reinterpret_cast<T const *>(recvWindow->data()),
            reinterpret_cast<T *>(buffer), count);
        ++sendWindow;
        ++recvWindow;
    }
}

template<typename T>
void pMR::RecursiveDoubling::AllReduce::reduceBroadcast(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count)
{
    if(mRDPartition >= 0)
    {
        // Reduce
        auto recvWindow = mRDRecvWindows.end();
        while(recvWindow != mRDRecvWindows.begin() + mRDPartition)
        {
            --recvWindow;
            recvWindow->post();
            recvWindow->wait();
            op(reinterpret_cast<T const *>(recvWindow->data()),
                reinterpret_cast<T *>(buffer), count);
        }
        if(mRDPartition != 0)
        {
            mRDSendWindows[mRDPartition - 1].post();
            mRDSendWindows[mRDPartition - 1].wait();

            // Broadcast
            mRDRecvWindows[mRDPartition - 1].post();
            mRDRecvWindows[mRDPartition - 1].wait();
            std::memcpy(buffer, mRDRecvWindows[mRDPartition - 1].data(),
                sizeof(T) * count);
        }
        auto sendWindow = mRDSendWindows.begin() + mRDPartition;
        while(sendWindow != mRDSendWindows.end())
        {
            sendWindow->post();
            sendWindow->wait();
            ++sendWindow;
        }
    }
}

#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING_ALLREDUCE_H
