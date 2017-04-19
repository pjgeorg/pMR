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

#include "target.hpp"
#include <stdexcept>

pMR::Target::Target(MPI_Comm const communicator, int const targetRank,
    int const uniqueSendID, int const uniqueRecvID, bool const null,
    bool const self)
    : mCommunicator{communicator}
    , mTarget{targetRank}
    , mUniqueSendID{uniqueSendID}
    , mUniqueRecvID{uniqueRecvID}
    , mNull{null}
    , mSelf{self}
{
    if(mNull && mSelf)
    {
        throw std::logic_error("pMR: Target specified as both null and self.");
    }
}

pMR::Target::Target(MPI_Comm const communicator, int const targetRank,
    int const uniqueSendID, int const uniqueRecvID)
    : mCommunicator{communicator}
    , mTarget{targetRank}
    , mUniqueSendID{uniqueSendID}
    , mUniqueRecvID{uniqueRecvID}
{
    queryTarget();
}

bool pMR::Target::isRemote() const
{
    return {!(mNull || mSelf)};
}

bool pMR::Target::isNull() const
{
    return {mNull};
}

bool pMR::Target::isSelf() const
{
    return {mSelf};
}

int pMR::Target::getTargetRank() const
{
    return {mTarget};
}

int pMR::Target::getUniqueSendID() const
{
    return {mUniqueSendID};
}

int pMR::Target::getUniqueRecvID() const
{
    return {mUniqueRecvID};
}

MPI_Comm pMR::Target::getMPICommunicator() const
{
    return {mCommunicator};
}

int pMR::Target::queryRank() const
{
    int ownRank;
    if(MPI_Comm_rank(mCommunicator, &ownRank))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }
    return {ownRank};
}

void pMR::Target::queryTarget()
{
    if(mTarget == MPI_PROC_NULL)
    {
        mNull = {true};
    }

    if(queryRank() == mTarget)
    {
        mSelf = {true};
    }
}
