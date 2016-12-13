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

#include "connection.hpp"
#include "target.hpp"
#include "../../misc/print.hpp"

extern "C" {
#include "pmr.h"

pMR_Connection *pMR_CreateConnection(
    MPI_Comm communicator, int targetRank, int uniqueSendID, int uniqueRecvID)
{
    try
    {
        return reinterpret_cast<pMR_Connection *>(
            new pMR::Connection(pMR::Target(
                communicator, {targetRank}, {uniqueSendID}, {uniqueRecvID})));
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
        return nullptr;
    }
}

void pMR_DestroyConnection(pMR_Connection *connection)
{
    try
    {
        delete reinterpret_cast<pMR::Connection *>(connection);
    }
    catch(std::exception const &e)
    {
        pMR::print(e.what());
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}
}
