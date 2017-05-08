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
#include <algorithm>
#include <sstream>
#include <vector>
extern "C" {
#include <omp.h>
}
#include "../allreduce.hpp"
#include "../communicator.hpp"
#include "../connection.hpp"
#include "../misc/allocator.hpp"
#include "../misc/random.hpp"
#include "../misc/time.hpp"
#include "../recvwindow.hpp"
#include "../sendwindow.hpp"
#include "mpi.hpp"
#include "parameter.hpp"

void runAllReduce(int argc, char **argv)
{
    // Default options
    std::uint64_t maxIter = {10000};
    std::uint64_t overallSize = {1024 * 1024 * 1024};
    std::uint32_t minMsgSize = {0};
    std::uint32_t maxMsgSize = {128 * 1024};
    std::uint32_t deltaMsgSize = 100;
    std::uint32_t minDeltaMsgSize = {4};
    std::uint32_t maxDeltaMsgSize = {4 * 1024};
    std::uint32_t granularity = {32};
    bool bitExact = {false};
    bool verify = {false};

    // Get additional parameter options
    parameterOption<std::uint64_t>(argv, argv + argc, "--maxIter", maxIter);
    parameterOption<std::uint64_t>(
        argv, argv + argc, "--overallSize", overallSize);
    parameterOption<std::uint32_t>(
        argv, argv + argc, "--minMsgSize", minMsgSize);
    parameterOption<std::uint32_t>(
        argv, argv + argc, "--maxMsgSize", maxMsgSize);

    parameterOption<std::uint32_t>(
        argv, argv + argc, "--deltaMsgSize", deltaMsgSize);
    parameterOption<std::uint32_t>(
        argv, argv + argc, "--minDeltaMsgSize", minDeltaMsgSize);
    parameterOption<std::uint32_t>(
        argv, argv + argc, "--maxDeltaMsgSize", maxDeltaMsgSize);
    parameterOption<std::uint32_t>(
        argv, argv + argc, "--granularity", granularity);

    if(minDeltaMsgSize > maxDeltaMsgSize)
    {
        printUsage();
    }

    if(parameterExists(argv, argv + argc, "--bitExact"))
    {
        bitExact = {true};
    }

    if(parameterExists(argv, argv + argc, "--verify"))
    {
        verify = {true};
    }

    // Prepare benchmark environment
    pMR::Communicator communicator(MPI_COMM_WORLD);

    // Print Benchmark Information
    printMaster("Benchmark:    AllReduce");
    printMaster("Processes:   ", communicator.size());
    printMaster("BitExact:    ", bitExact);
    printMaster("Verify:      ", verify);
    printMaster();

    printMaster(" MPIRank Size[By] WSize[By] Iteration  Time/It[s]");

    // Loop Message Sizes
    auto msgSize = minMsgSize;
    while(msgSize <= maxMsgSize)
    {
        auto msgCount = msgSize / sizeof(float);
        msgSize = msgCount * sizeof(float);

        // Calculate iterations count
        auto iter = maxIter;
        if(msgSize)
        {
            iter = std::min(iter,
                overallSize / msgSize /
                    static_cast<std::uint64_t>(communicator.size()));
        }

        pMR::AllReduce allReduce(communicator,
            std::ceil(static_cast<double>(msgSize) / granularity) *
                granularity);

        std::vector<float, pMR::Allocator<float>> vData;
        std::vector<float, pMR::Allocator<float>> vSum;

        if(verify)
        {
            vData.resize(msgCount + pMR::cacheLineSize<float>());
            vSum.resize(msgCount + pMR::cacheLineSize<float>());
            std::fill(vData.begin(), vData.end(), 0);
            std::fill_n(vSum.rbegin(), pMR::cacheLineSize<float>(), 0);
        }
        else
        {
            vSum.resize(msgCount);
        }

        std::generate_n(vSum.begin(), msgCount, pMR::getRandomNumber<float>);
        std::for_each(vSum.begin(), vSum.end(),
            [&communicator](float &f) { f /= communicator.size(); });

        double time = -pMR::getTimeInSeconds();
        for(auto i = decltype(iter){0}; i != iter; ++i)
        {
            allReduce.insert(vSum.cbegin(), msgCount);
            if(bitExact)
            {
                allReduce.executeBit<float>(pMR::Operation::Sum, msgCount);
            }
            else
            {
                allReduce.execute<float>(pMR::Operation::Sum, msgCount);
            }
            allReduce.extract(vSum.begin(), msgCount);

            if(verify)
            {
                std::for_each(vSum.begin(), vSum.end(),
                    [&communicator](float &f) { f /= communicator.size(); });
                if(i == 0)
                {
                    std::copy(vSum.begin(), vSum.end(), vData.begin());
                }

                if(vSum != vData)
                {
                    throw std::runtime_error("Verification check failed!");
                }
            }
        }
        time += pMR::getTimeInSeconds();

        std::ostringstream oss;
        oss << std::setw(8) << getRank(MPI_COMM_WORLD) << " " << std::setw(8)
            << msgSize << "  " << std::setw(8) << allReduce.size() << " "
            << std::setw(8) << iter << " " << std::scientific << time / iter
            << std::endl;
        std::cout << oss.str();

        // Increment msgSize for next loop
        auto tmpSize = (msgSize * (100 + deltaMsgSize)) / 100;
        tmpSize -= tmpSize % minDeltaMsgSize;
        if(tmpSize - msgSize < minDeltaMsgSize)
        {
            tmpSize = msgSize + minDeltaMsgSize;
        }
        if(tmpSize - msgSize > maxDeltaMsgSize)
        {
            tmpSize = msgSize + maxDeltaMsgSize;
        }
        msgSize = tmpSize;
    }

    // Exit application
    finalize();
}
