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
#include "thread.hpp"
#include "../../backends/backend.hpp"
#include "../../backends/mpi/threadsupport.hpp"

pMR::mpi::Connection::Connection(Target const &target)
    : mCommunicator{target.getMPICommunicator()}
    , mTargetRank{target.getTargetRank()}
{
    mSendTag =
    { static_cast<int>(reinterpret_cast<std::intptr_t>(this) %
#ifdef MPI_TAG_NARROW
        std::numeric_limits<std::uint16_t>::max()) }
#else
        std::numeric_limits<int>::max()) }
#endif // MPI_TAG_NARROW

    if(backend::threadMultiple())
    {
        mMultipleThreadSupport = {true};
    }
    else
    {
        if(!backend::threadSerialized() && !thread::isThreaded())
        {
            throw std::runtime_error(
                "pMR: Require at least MPI_THREAD_SERIALIZED Thread support");
        }
    }

    // Exchange a (hopefully) unique message tag with remote
    backend::exchange(target, mSendTag, mRecvTag);
}

MPI_Comm pMR::mpi::Connection::getCommunicator() const
{
    return {mCommunicator};
}

int pMR::mpi::Connection::getTargetRank() const
{
    return {mTargetRank};
}

int pMR::mpi::Connection::getSendTag() const
{
    return {mSendTag};
}

int pMR::mpi::Connection::getRecvTag() const
{
    return {mRecvTag};
}

bool pMR::mpi::Connection::multipleThreadSupport() const
{
    return {mMultipleThreadSupport};
}
