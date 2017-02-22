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

#include "threadsupport.hpp"
#include <stdexcept>
extern "C" {
#include <mpi.h>
}

pMR::backend::ThreadSupport::ThreadSupport()
{
    if(sLevel == ThreadLevel::Unknown)
    {
        int provided;
        if(MPI_Query_thread(&provided) != MPI_SUCCESS)
        {
            throw std::runtime_error(
                "pMR: Unable to query MPI Thread support.");
        }

        switch(provided)
        {
            case MPI_THREAD_MULTIPLE:
                sLevel = ThreadLevel::Multiple;
                break;
            case MPI_THREAD_SERIALIZED:
                sLevel = ThreadLevel::Serialized;
                break;
            case MPI_THREAD_FUNNELED:
                sLevel = ThreadLevel::Funneled;
                break;
            case MPI_THREAD_SINGLE:
                sLevel = ThreadLevel::Single;
                break;
            default:
                throw std::runtime_error("pMR: Unknown MPI thread level.");
        }
    }
}

enum pMR::ThreadLevel pMR::backend::ThreadSupport::getLevel() const
{
    return {sLevel};
}

std::mutex pMR::backend::ThreadSupport::Mutex;

enum pMR::ThreadLevel pMR::backend::ThreadSupport::sLevel =
    pMR::ThreadLevel::Unknown;
