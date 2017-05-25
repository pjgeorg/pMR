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

#include "local.hpp"
#include <stdexcept>
#include "../backend.hpp"

MPI_Comm pMR::Backend::getLocalCommunicator()
{
    MPI_Comm localCommunicator;

    if(MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0,
           MPI_INFO_NULL, &localCommunicator) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to get local communicator");
    }

    return {localCommunicator};
}

int pMR::Backend::getLocalProcessID()
{
    int rank;

    if(MPI_Comm_rank(getLocalCommunicator(), &rank) != MPI_SUCCESS)
    {
        throw std::runtime_error(
            "pMR: Unable to get local communicator rank ID");
    }

    return {rank};
}

int pMR::Backend::getLocalNumProcesses()
{
    int size;

    if(MPI_Comm_size(getLocalCommunicator(), &size) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to get local communicator size");
    }

    return {size};
}
