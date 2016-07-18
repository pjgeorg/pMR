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

#include "global.hpp"
#include "../mpi.hpp"
#include "../reduce.hpp"
#include "communicator.hpp"
#include "connection.hpp"
#include "sendwindow.hpp"
#include "recvwindow.hpp"
#include "allreduce.hpp"

void runGlobalTest()
{
    std::vector<int> geom(1, 0);
    std::vector<int> periodic(1, 1);
    pMR::Communicator communicator(MPI_COMM_WORLD, geom, periodic);

    pMR::AllReduce allReduce(communicator, sizeof(float));

    for(int iter = 0; iter != 1e3; ++iter)
    {
        allReduce.data<float>()[0] = communicator.ID();
        allReduce.execute(&plus, 1);
    }

    // Verify data
    float sum = 0;
    for(int proc = 0; proc != communicator.size(); ++proc)
    {
        sum += proc;
    }

    if(sum != allReduce.data<float>()[0])
    {
        pMR::print(static_cast<std::uint32_t>(getRank(MPI_COMM_WORLD)),
                "Allreduce FAILED!"); 
    }

    allReduce.execute(&plus, 1);

    sum *= communicator.size();

    if(sum != allReduce.data<float>()[0])
    {
        pMR::print(static_cast<std::uint32_t>(getRank(MPI_COMM_WORLD)),
                "Allreduce FAILED!"); 
        abort(1);
    }

    printMaster("Allreduce: SUCCESS!");

    // Exit application
    finalize();
}
