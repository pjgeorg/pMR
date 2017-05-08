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

static constexpr int cBlocking = RECURSIVE_DOUBLING_BLOCKING;
static constexpr int cDomainSize = RECURSIVE_DOUBLING_DOMAIN_SIZE;

static constexpr bool isPowerOfTwo(int x)
{
    return x && ((x & (x - 1)) == 0);
}

static_assert(isPowerOfTwo(cBlocking),
    "Recursive Doubling blocking factor is not a power of two.");

pMR::RecursiveDoubling::AllReduce::AllReduce(
    Communicator const &communicator, void *buffer, size_type const sizeByte)
{
    // Recursive doubling only works for a power of two number of processes.
    // For the non-power of two case we do:
    //  1. Reduce withing a domain
    //  2. A pre step to reduce the number to the nearest lower power of two.
    //  3. Recursive-doubling with the reduced number of processes.
    //  4. A post step to scatter the result to not involved processes.
    //  5. Broadcast within a domain
    //  Note: Step 1. and  5. are optional to first reduce within a domain.

    auto numProcesses = communicator.size();
    auto processID = communicator.ID();

    // Padded size for contigious buffers (each chunk is aligned)
    auto padSize = alignedSize(sizeByte);
    auto offsetBuffer = decltype(mDomainBuffers.size()){0};

    auto domainSize = cDomainSize;
    if(cDomainSize > 1)
    {
        if(numProcesses % cDomainSize == 0)
        {
            if(processID % cDomainSize == 0)
            {
                mDomainBuffers.resize((cDomainSize - 1) * padSize);
                mDomainRoot = {true};
                offsetBuffer = 0;
                for(auto target = processID + 1;
                    target < processID + cDomainSize; ++target)
                {
                    mDomainConnections.emplace_back(
                        communicator.getTarget(target));

                    mDomainSendWindows.emplace_back(
                        mDomainConnections.back(), buffer, sizeByte);
                    mDomainRecvWindows.emplace_back(mDomainConnections.back(),
                        static_cast<void *>(
                            mDomainBuffers.data() + offsetBuffer),
                        sizeByte);
                    offsetBuffer += padSize;
                }
            }
            else
            {
                auto target = processID - (processID % cDomainSize);
                mDomainConnections.emplace_back(communicator.getTarget(target));
                mDomainSendWindows.emplace_back(
                    mDomainConnections.back(), buffer, sizeByte);
                mDomainRecvWindows.emplace_back(
                    mDomainConnections.back(), buffer, sizeByte);
                return;
            }

            processID /= cDomainSize;
            numProcesses /= cDomainSize;
        }
        else
        {
            domainSize = 1;
        }
    }

    // Get nearest power of two less than or equal to number of processes.
    // Also gets depth log_2(n) of the algorithm
    int Po2Processes = 0x1;
    int depth = -1;
    while(Po2Processes <= numProcesses)
    {
        Po2Processes <<= 1;
        ++depth;
    }
    Po2Processes >>= 1;

    auto blocking = decltype(cBlocking){1};
    if(numProcesses != Po2Processes)
    {
        // Need to perform pre step to reduce number of processes.

        // Do a blocking of ranks only if possible
        if((numProcesses - Po2Processes) % cBlocking == 0)
        {
            blocking = cBlocking;
        }

        if(processID % (numProcesses / blocking) <
            2 * (numProcesses - Po2Processes) / blocking)
        {
            if(processID % (numProcesses / blocking) % 2 == 0)
            {
                auto target = (processID + 1) * domainSize;
                mPreConnection = std::unique_ptr<pMR::Connection>(
                    new pMR::Connection(communicator.getTarget(target)));

                mPreSendWindow = std::unique_ptr<pMR::SendMemoryWindow>(
                    new pMR::SendMemoryWindow(
                        *mPreConnection, buffer, sizeByte));
                mPreBuffer.resize(sizeByte);
                mPreRecvWindow = std::unique_ptr<pMR::RecvMemoryWindow>(
                    new pMR::RecvMemoryWindow(*mPreConnection,
                        static_cast<void *>(mPreBuffer.data()), sizeByte));

                processID = (processID / (numProcesses / blocking)) *
                        (Po2Processes / blocking) +
                    ((processID % (numProcesses / blocking)) / 2);
            }
            else
            {
                auto target = (processID - 1) * domainSize;
                mPreConnection = std::unique_ptr<pMR::Connection>(
                    new pMR::Connection(communicator.getTarget(target)));
                mPreSendWindow = std::unique_ptr<pMR::SendMemoryWindow>(
                    new pMR::SendMemoryWindow(
                        *mPreConnection, buffer, sizeByte));
                mPreRecvWindow = std::unique_ptr<pMR::RecvMemoryWindow>(
                    new pMR::RecvMemoryWindow(
                        *mPreConnection, buffer, sizeByte));
                return;
            }
        }
        else
        {
            processID -= (numProcesses - Po2Processes) / blocking *
                (processID / (numProcesses / blocking) + 1);
        }
    }

    // Setup recursive-doubling
    mRDPartition = depth;
    for(auto c = processID + 1; c <= Po2Processes / 2; c <<= 1)
    {
        --mRDPartition;
    }

    mRDBuffers.resize(depth * padSize);

    // Distance to the target process ID of a certain level
    auto distance = decltype(depth){0x1};

    for(auto level = 0; level != depth; ++level)
    {
        auto target = processID ^ distance;

#ifdef RECURSIVE_DOUBLING_FBT
        if(level == depth - 1)
        {
            if(Po2Processes % 4 == 0)
            {
                if(processID / (Po2Processes / 4) % 2 == 0)
                {
                    target = processID - Po2Processes / 4 + Po2Processes;
                }
                else
                {
                    target = processID + Po2Processes / 4;
                }
                target %= Po2Processes;
            }
        }
#endif // RECURSIVE_DOUBLING_FBT

        // Restore real process ID of target
        if(target % (Po2Processes / blocking) <
            1 * (numProcesses - Po2Processes) / blocking)
        {
            target = (target / (Po2Processes / blocking)) *
                    (numProcesses / blocking) +
                ((target % (Po2Processes / blocking)) * 2);
        }
        else
        {
            target += (numProcesses - Po2Processes) / blocking *
                (target / (numProcesses / blocking) + 1);
        }
        target *= domainSize;

        mRDConnections.emplace_back(communicator.getTarget(target));
        mRDSendWindows.emplace_back(mRDConnections.back(), buffer, sizeByte);
        mRDRecvWindows.emplace_back(mRDConnections.back(),
            static_cast<void *>(mRDBuffers.data() + level * padSize), sizeByte);

        // Calculate distance for next level
        distance <<= 1;
    }
}

void pMR::RecursiveDoubling::AllReduce::initWindows()
{
    for(auto &window : mDomainSendWindows)
    {
        window.init();
    }
    for(auto &window : mDomainRecvWindows)
    {
        window.init();
    }
    if(mPreConnection)
    {
        mPreSendWindow->init();
        mPreRecvWindow->init();
    }
    for(auto &window : mRDSendWindows)
    {
        window.init();
    }
    for(auto &window : mRDRecvWindows)
    {
        window.init();
    }
}

void pMR::RecursiveDoubling::AllReduce::postBroadcast()
{
    if(mPreConnection)
    {
        if(mRDConnections.size() == 0)
        {
            mPreRecvWindow->post();
            mPreRecvWindow->wait();
        }
        else
        {
            mPreSendWindow->post();
            mPreSendWindow->wait();
        }
    }
}

void pMR::RecursiveDoubling::AllReduce::domainBroadcast()
{
    if(mDomainRoot)
    {
        for(auto &window : mDomainSendWindows)
        {
            window.post();
        }
        for(auto &window : mDomainSendWindows)
        {
            window.wait();
        }
    }
    else
    {
        for(auto &window : mDomainRecvWindows)
        {
            window.post();
            window.wait();
        }
    }
}
