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

#include "../backend.hpp"
#include <stdexcept>
extern "C"
{
#include <mpi.h>
}
#include "target.hpp"
#include "threadsupport.hpp"
#include "../../threads/thread.hpp"

void pMR::backend::exchange(Target const &target,
        void const *sendBuffer, void *recvBuffer, std::uint32_t const sizeByte)
{
    if(threadMultiple() || !thread::isThreaded() || thread::isSerialized())
    {
        if(MPI_Sendrecv(sendBuffer, sizeByte, MPI_BYTE,
                target.getTargetRank(),
                target.getUniqueSendID(),
                recvBuffer, sizeByte, MPI_BYTE,
                target.getTargetRank(),
                target.getUniqueRecvID(), 
                target.getMPICommunicator(),
                MPI_STATUS_IGNORE) != MPI_SUCCESS)
        {
            throw std::runtime_error(
                    "pMR: Unable to exchange connection data.");
        }
    }
    else
    {
        if(threadSerialized())
        {
            MPI_Request sendRequest;
            MPI_Request recvRequest;
            {
                thread::ScopedLock scopedLock;
                if(MPI_Isend(sendBuffer, sizeByte, MPI_BYTE,
                        target.getTargetRank(),
                        target.getUniqueSendID(),
                        target.getMPICommunicator(),
                        &sendRequest) != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
                if(MPI_Irecv(recvBuffer, sizeByte, MPI_BYTE,
                        target.getTargetRank(),
                        target.getUniqueRecvID(),
                        target.getMPICommunicator(),
                        &recvRequest) != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
            }
    
            int flag = false;
            while(!flag)
            {
                thread::ScopedLock scopedLock;
                if(MPI_Test(&sendRequest, &flag, MPI_STATUS_IGNORE)
                        != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
            }
            flag = false;
            while(!flag)
            {
                thread::ScopedLock scopedLock;
                if(MPI_Test(&recvRequest, &flag, MPI_STATUS_IGNORE)
                        != MPI_SUCCESS)
                {
                    throw std::runtime_error(
                            "pMR: Unable to exchange connection data");
                }
            }
        }
        else
        {
            throw std::runtime_error("pMR: Require at least "
                    "MPI_THREAD_SERIALIZED Thread support");
        }
    }
}
