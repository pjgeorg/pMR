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

#ifndef pMR_TOOL_MPI_H
#define pMR_TOOL_MPI_H

extern "C"
{
#include <mpi.h>
}
#include "../misc/print.hpp"

void init(int argc, char **argv);
void finalize(int exitCode = 0);
void abort(int exitCode = 0);

int getRank(MPI_Comm communicator);

template<typename ...Types>
void printMaster(Types const &...args)
{
    if(getRank(MPI_COMM_WORLD) == 0)
    {
        pMR::print(args...);
    }
}
#endif // pMR_TOOL_MPI_H
