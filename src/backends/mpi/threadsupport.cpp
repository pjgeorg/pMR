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
#include "../../misc/singleton.hpp"

pMR::backend::ThreadSupport::ThreadSupport()
{
    int provided;
    if(MPI_Query_thread(&provided) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to query MPI Thread support.");
    }

    if(provided == MPI_THREAD_MULTIPLE)
    {
        mMultiple = true;
    }

    if(provided == MPI_THREAD_SERIALIZED)
    {
        mSerialized = true;
    }
}

bool pMR::backend::ThreadSupport::multiple() const
{
    return mMultiple;
}

bool pMR::backend::ThreadSupport::serialized() const
{
    return mSerialized;
}

bool pMR::backend::threadMultiple()
{
    return Singleton<ThreadSupport>::Instance().multiple();
}

bool pMR::backend::threadSerialized()
{
    return Singleton<ThreadSupport>::Instance().serialized();
}
