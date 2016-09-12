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

#include "benchmark.hpp"
#include <array>
#include <sstream>
#include <algorithm>
extern "C"
{
#include <omp.h>
}
#include "parameter.hpp"
#include "mpi.hpp"
#include "../misc/allocator.hpp"
#include "../misc/time.hpp"
#include "communicator.hpp"
#include "connection.hpp"
#include "sendwindow.hpp"
#include "recvwindow.hpp"
#include "allreduce.hpp"
#include "reduce.hpp"

void runBenchmark(int argc, char **argv)
{
    // Default options
    int dim = 1;
    std::vector<int> geom;
    std::vector<int> periodic;
    std::uint64_t maxIter = 1e4;
    std::uint64_t overallSize = 1024 * 1024 * 1024;
    std::uint32_t minMsgSize = 0;
    std::uint32_t maxMsgSize = 128 * 1024;
    std::uint32_t deltaMsgSize = 4 * 1024;
    std::uint32_t granularity = 4;
    bool bufferedSend = false;
    bool bufferedRecv = false;

    // Get additional parameter options
    parameterOption(argv, argv + argc, "--dim", dim); 
    geom.resize(dim, 0);
    periodic.resize(dim, 1);
    parameterOption(argv, argv + argc, "--geom", geom);
    parameterOption(argv, argv + argc, "--periodic", periodic);
    parameterOption<std::uint64_t>(argv, argv + argc, "--maxIter", maxIter);
    parameterOption<std::uint64_t>(argv, argv + argc, "--overallSize",
            overallSize);
    parameterOption<std::uint32_t>(argv, argv + argc, "--minMsgSize",
            minMsgSize);
    parameterOption<std::uint32_t>(argv, argv + argc, "--maxMsgSize",
            maxMsgSize);
    parameterOption<std::uint32_t>(argv, argv + argc, "--deltaMsgSize",
            deltaMsgSize);
    parameterOption<std::uint32_t>(argv, argv + argc, "--granularity",
            granularity);
    if(parameterExists(argv, argv + argc, "--bufferedSend"))
    {
        bufferedSend = true;
    }
    if(parameterExists(argv, argv + argc, "--bufferedRecv"))
    {
        bufferedRecv = true;
    }

    // Prepare benchmark environment
    pMR::Communicator communicator(MPI_COMM_WORLD, geom, periodic);

    std::string benchmark;
    parameterOption<std::string>(argv, argv + argc, "--benchmark", benchmark);
    if(benchmark == "allreduce")
    {
        // Print Benchmark Information
        printMaster("Benchmark:   ", benchmark);
        printMaster("Processes:   ", communicator.size());
        printMaster();
    
        printMaster(" MPIRank Size[By] WSize[By] Iteration  Time/It[s]");

        deltaMsgSize = (deltaMsgSize / sizeof(float)) * sizeof(float);
        if(deltaMsgSize == 0)
        {
            deltaMsgSize += sizeof(float);
        }

        // Loop Message Sizes
        auto msgSize = minMsgSize;
        while(msgSize <= maxMsgSize)
        {
            msgSize = (msgSize / sizeof(float)) * sizeof(float);
            // Calculate iterations count
            auto iter = maxIter;
            if(msgSize)
            {
                iter = std::min(iter, overallSize / msgSize /
                        static_cast<std::uint64_t>(communicator.size()));
            }

            pMR::AllReduce allReduce(communicator,
                    std::ceil(static_cast<double>(msgSize) / granularity) *
                    granularity);

            std::vector<float> vSum;
            vSum.resize(msgSize / sizeof(float));
            double time = -pMR::getTimeInSeconds();
            for(decltype(iter) i = 0; i != iter; ++i)
            {
                allReduce.insert<float>(vSum.cbegin(), msgSize / sizeof(float));
                allReduce.execute(&plus, msgSize / sizeof(float));
                allReduce.extract<float>(vSum.begin(), msgSize / sizeof(float));
            }
            time += pMR::getTimeInSeconds();
            pMR::print(static_cast<std::uint32_t>(getRank(MPI_COMM_WORLD)),
                    msgSize, allReduce.size<unsigned char>(),
                    iter, time / iter);
            msgSize += deltaMsgSize;
        }

        // Exit application
        finalize();
    }
    if(benchmark != "exchange" and benchmark != "sendrecv")
    {
        printUsage();
    }
    
    // Establish connections
    std::vector<pMR::Connection> connections;

    // Store number of real data transfers per iteration
    unsigned int nDataTransfers = 0;

    for(int n = 0; n != communicator.dimensions(); ++n)
    {
        if(communicator.size(n) > 1)
        {
            pMR::Target lNeighbor = communicator.getNeighbor(n, -1);
            pMR::Target rNeighbor = communicator.getNeighbor(n, +1);
            if(!lNeighbor.isNull())
            {
                ++nDataTransfers;
            }
            if(!rNeighbor.isNull() && benchmark != "sendrecv")
            {
                ++nDataTransfers;
            }

            if(communicator.coordinate(n) % 2 == 1)
            {
                connections.emplace_back(lNeighbor);
                connections.emplace_back(rNeighbor);
            }
            else
            {
                connections.emplace_back(rNeighbor);
                connections.emplace(connections.end() - 1, lNeighbor);
            }
        }
    }

    // Print Benchmark Information
    printMaster("Benchmark:   ", benchmark);
    printMaster("Processes:   ", communicator.size());
    printMaster("Dimensions:  ", static_cast<int>(connections.size()/2), "/",
            communicator.dimensions());
    printMaster("Topology:    ", communicator.topology());
    printMaster("Periodic:    ", communicator.periodic());
    printMaster("BufferedSend:", bufferedSend);
    printMaster("BufferedRecv:", bufferedRecv);
    printMaster();
    
    printMaster(" MPIRank Size[By] Iteration  Time/It[s] Bandwidth[MB/s]");

    // Loop Message Sizes
    auto msgSize = minMsgSize;
    while(msgSize <= maxMsgSize)
    {
        std::vector<std::vector<unsigned char,
            pMR::AlignedAllocator<unsigned char>>> sendBuffers, recvBuffers;
        std::vector<pMR::SendWindow<unsigned char>> sendWindows;
        std::vector<pMR::RecvWindow<unsigned char>> recvWindows;
        
        // Create Buffers and Windows
        for(decltype(connections.size()) c = 0; c != connections.size(); ++c)
        {
            sendBuffers.emplace_back(msgSize);
            if(bufferedSend)
            {
                sendWindows.emplace_back(connections[c], msgSize);
            }
            else
            {
                sendWindows.emplace_back(connections[c],
                        sendBuffers.back().data(), msgSize);
            }

            if(benchmark == "sendrecv")
            {
                ++c;
            }

            recvBuffers.emplace_back(msgSize);
            if(bufferedRecv)
            {
                recvWindows.emplace_back(connections[c], msgSize);
            }
            else
            {
                recvWindows.emplace_back(connections[c],
                        recvBuffers.back().data(), msgSize);
            }
        }

        // Go to next loop if nothing to do
        if(sendWindows.size() == 0 and recvWindows.size() == 0)
        {
            // Increment msgSize for next loop
            msgSize += deltaMsgSize;
            continue;
        }

        // Calculate iterations count
        auto iter = maxIter;
        if(msgSize)
        {
            iter = std::min(iter, overallSize / msgSize /
                    static_cast<std::uint64_t>(sendWindows.size()));
        }

        double time = -pMR::getTimeInSeconds();
        for(decltype(iter) i = 0; i != iter; ++i)
        {
            for(auto &recvWindow : recvWindows)
            {
                recvWindow.init();
            }
            for(auto &sendWindow : sendWindows)
            {
                sendWindow.init();
            }
            if(bufferedSend)
            {
                for(decltype(sendWindows.size()) s = 0; s != sendWindows.size();
                        ++s)
                {
                    sendWindows[s].insert(sendBuffers[s].cbegin());
                }
            }
            for(auto &sendWindow : sendWindows)
            {
                sendWindow.post();
            }
            for(auto &recvWindow : recvWindows)
            {
                recvWindow.post();
            }
            for(auto &sendWindow : sendWindows)
            {
                sendWindow.wait();
            }
            for(auto &recvWindow : recvWindows)
            {
                recvWindow.wait();
            }
            if(bufferedRecv)
            {
                for(decltype(recvWindows.size()) r = 0; r != recvWindows.size();
                        ++r)
                {
                    recvWindows[r].extract(recvBuffers[r].begin());
                }
            }

        }
        time += pMR::getTimeInSeconds();
        pMR::print(static_cast<std::uint32_t>(getRank(MPI_COMM_WORLD)),
                msgSize, iter, time / iter,
                msgSize * iter * nDataTransfers / time / 1024 / 1024 );

        // Increment msgSize for next loop
        msgSize += deltaMsgSize;
    }

    // Exit application
    finalize();
}
