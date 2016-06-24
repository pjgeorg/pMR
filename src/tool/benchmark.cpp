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
    if(parameterExists(argv, argv + argc, "--bufferedSend"))
    {
        bufferedSend = true;
    }
    if(parameterExists(argv, argv + argc, "--bufferedRecv"))
    {
        bufferedRecv = true;
    }
    std::string benchmark;
    parameterOption<std::string>(argv, argv + argc, "--benchmark", benchmark);
    if(benchmark != "exchange" and benchmark != "sendrecv")
    {
        printUsage();
    }

    // Prepare benchmark environment
    pMR::Communicator communicator(MPI_COMM_WORLD, geom, periodic);
    
    // Establish connections
    std::vector<pMR::Connection> connections;

    for(int n = 0; n != communicator.dimensions(); ++n)
    {
        if(communicator.size(n) > 1)
        {
            if(communicator.coordinate(n) % 2 == 1)
            {
                connections.emplace_back(communicator.getNeighbor(n, -1));
                connections.emplace_back(communicator.getNeighbor(n, +1));
            }
            else
            {
                connections.emplace_back(communicator.getNeighbor(n, +1));
                connections.emplace(connections.end() - 1,
                        communicator.getNeighbor(n, -1));
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
        std::vector<std::vector<unsigned char, pMR::AlignedAllocator<unsigned char>>>
            sendBuffers, recvBuffers;
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

        // Increment msgSize for next loop
        msgSize += deltaMsgSize;

        // Go to next loop if nothing to do
        if(sendWindows.size() == 0 and recvWindows.size() == 0)
        {
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
                msgSize * iter * sendWindows.size() / time / 1024 / 1024 );
    }

    // Exit application
    finalize();
}
