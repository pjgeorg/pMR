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

#include "allreduce.hpp"
#include <cmath>

#include <iostream>

pMR::AllReduce::AllReduce(Communicator const communicator,
        std::uint32_t const sizeByte)
    :   mSizeByte(std::ceil(static_cast<double>(sizeByte) / alignment)
            * alignment)
{
    // Recursive doubling only works for a power of two number of processes.
    // For the non-power of two case we do:
    //  1. A pre step to reduce the number to the nearest lower power of two.
    //  2. Recursive-doubling with the reduced number of processes.
    //  3. A post step to scatter the result to not involved processes.

    // Get nearest power of two less than or equal to number of ranks.
    // Also gets depth log_2(n) of the algorithm
    int Po2Processes = 0x1;
    int depth = -1;
    while(Po2Processes <= communicator.size())
    {
        Po2Processes <<=1;
        ++depth;
    }
    Po2Processes >>= 1;

    // Number of required buffers
    // Start at 1, as we always require at least one to store the local data.
    int numBuffers = 1;

    // processID for recursive-doubling
    // -1 indicates that the process is not part of recursive-doubling.
    int processID = communicator.ID();

    // Check if number of processes is not a power of two
    if(communicator.size() - Po2Processes != 0)
    {
        // Need to perform pre step to reduce number of processes.
        // This is done by sending the data of the first odd size() - Po2
        // processes to the first even. Afterwards processes that should
        // participate in recursive doubling are assigned a new ID.
        if(communicator.ID() < 2 * (communicator.size() - Po2Processes))
        {
            if(communicator.ID() % 2 == 0)
            {
                // Establish connection to odd rank (+1)
                mPPConnection = std::unique_ptr<pMR::Connection>(
                        new pMR::Connection(communicator.getTarget(
                                communicator.ID() + 1)));

                // Assign new process ID
                processID /= 2;

                // Increment numBuffers by 1 to receive data
                ++numBuffers;
            }
            else
            {
                // Establish connection to even rank (-1)
                mPPConnection = std::unique_ptr<pMR::Connection>(
                        new pMR::Connection(communicator.getTarget(
                                communicator.ID() - 1)));

                // Assign new process ID
                processID = -1;
            }
        }
        else
        {
            // Assign new process ID
            processID -= communicator.size() - Po2Processes;
        }
    }

    // Setup recursive-doubling
    if(processID != -1)
    {
        // Increment numBuffers by depth for multi-buffering.
        numBuffers += depth;

        // Distance to the target process ID of a certain level
        int distance = 0x1;

        for(int level = 0; level != depth; ++level)
        {
            int target = processID ^ distance;

            // Restore real process ID of target
            if(target < communicator.size() - Po2Processes)
            {
                target *= 2;
            }
            else
            {
                target += communicator.size() - Po2Processes;
            }

            // Establish connection to target
            mRDConnections.emplace_back(communicator.getTarget(target));

            // Calculate distance for next level
            distance <<= 1;
        }
    }

    // Current buffer offset in contigious buffer
    decltype(mBuffers.size()) offsetBuffer = 0;

    // Allocate contigious buffer(s)
    mBuffers.resize(numBuffers * mSizeByte);

    // Setup Memory Windows for post and pre step
    if(mPPConnection)
    {
        mPPSendWindow = std::unique_ptr<pMR::SendMemoryWindow>(
                new pMR::SendMemoryWindow(*mPPConnection,
                    static_cast<void*>(mBuffers.data() + 0), mSizeByte));

        // For processes only involved in pre and post step we use the same
        // buffer for send (pre) and receive (post) memory window.
        if(processID != -1)
        {
            offsetBuffer += mSizeByte;
        }

        mPPRecvWindow = std::unique_ptr<pMR::RecvMemoryWindow>(
                new pMR::RecvMemoryWindow(*mPPConnection,
                    static_cast<void*>(mBuffers.data() + offsetBuffer),
                    mSizeByte));
    }

    // Setup Memory Windows for recursive-doubling
    for(auto const &connection : mRDConnections)
    {
        // Use the same send buffer for all connections.
        mRDSendWindows.emplace_back(connection,
                static_cast<void*>(mBuffers.data() + 0), mSizeByte);

        // Increment offset by message size. I.e. use a different receive
        // buffer for each connection.
        offsetBuffer += mSizeByte;
        mRDRecvWindows.emplace_back(connection,
                static_cast<void*>(mBuffers.data() + offsetBuffer), mSizeByte);
    }
}
