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

#include "mpi.hpp"
#include <cstdlib>
#include <stdexcept>
#include "../../config.hpp"
#include "../../misc/print.hpp"

void init(int argc, char **argv)
{
    int required = []() {
        if(pMR::cThreadLevel <= pMR::ThreadLevel::Single)
        {
            return MPI_THREAD_SINGLE;
        }
        if(pMR::cThreadLevel <= pMR::ThreadLevel::Funneled)
        {
            return MPI_THREAD_FUNNELED;
        }
        if(pMR::cThreadLevel <= pMR::ThreadLevel::Serialized)
        {
            return MPI_THREAD_SERIALIZED;
        }

        return MPI_THREAD_MULTIPLE;
    }();

    int provided;

    if(MPI_Init_thread(&argc, &argv, required, &provided))
    {
        throw std::runtime_error("pMR: Unable to initialize MPI.");
    }
}

void finalize(int exitCode)
{
    MPI_Finalize();
    std::exit(exitCode);
}

void abort(int exitCode)
{
    MPI_Abort(MPI_COMM_WORLD, exitCode);
    std::exit(exitCode);
}

int getRank(MPI_Comm communicator)
{
    int rank;
    if(MPI_Comm_rank(communicator, &rank))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }

    return rank;
}
