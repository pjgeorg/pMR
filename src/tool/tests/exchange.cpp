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

#include "exchange.hpp"
#include "../mpi.hpp"
#include "communicator.hpp"
#include "connection.hpp"
#include "sendwindow.hpp"
#include "recvwindow.hpp"
#include "allreduce.hpp"

void runExchangeTest()
{
    for(int dim = 1; dim != 5; ++dim)
    { 
        std::vector<int> geom(dim, 0);
        std::vector<int> periodic(dim, 1);
        pMR::Communicator communicator(MPI_COMM_WORLD, geom, periodic);
    
        std::vector<pMR::Connection> connections;
    
        std::vector<pMR::SendWindow<int>> sendWindows;
        std::vector<pMR::RecvWindow<int>> recvWindows;
        
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
                sendWindows.emplace_back(connections[2 * n + 0],
                        2 * communicator.dimensions());
                recvWindows.emplace_back(connections[2 * n + 0],
                        2 * communicator.dimensions());
                for(int d = 0; d != communicator.dimensions(); ++d)
                {
                    sendWindows.back()[d] = communicator.coordinate(d);
                    sendWindows.back()[communicator.dimensions() + d] = 0;
                    if(d == n)
                    {
                        sendWindows.back()[d] -= 1;
                        sendWindows.back()[d] += communicator.size(d);
                        sendWindows.back()[d] %= communicator.size(d);
                    }
                }
                sendWindows.emplace_back(connections[2 * n + 1],
                        2 * communicator.dimensions());
                recvWindows.emplace_back(connections[2 * n + 1],
                        2 * communicator.dimensions());
                for(int d = 0; d != communicator.dimensions(); ++d)
                {
                    sendWindows.back()[d] = communicator.coordinate(d);
                    sendWindows.back()[communicator.dimensions() + d] = 0;
                    if(d == n)
                    {
                        sendWindows.back()[d] += 1;
                        sendWindows.back()[d] %= communicator.size(d);
                    }
                }
            }
        }

        for(auto &recvWindow : recvWindows)
        {
            recvWindow.init();
        }
        for(auto &sendWindow : sendWindows)
        {
            sendWindow.init();
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

        // Verify data
        for(unsigned int n = 0; n != connections.size(); ++n)
        {
            for(int d = 0; d != communicator.dimensions(); ++d)
            {
                if(recvWindows[n][d] != communicator.coordinate(d) ||
                        recvWindows[n][d + communicator.dimensions()] != 0)
                {
                    pMR::print(
                            static_cast<std::uint32_t>(getRank(MPI_COMM_WORLD)),
                            "Exchange FAILED!");
                    abort(1);
                }
            }
        }
    }

    printMaster("Exchange: SUCCESS!");
}
