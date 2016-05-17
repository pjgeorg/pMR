#include "target.hpp"
#include <stdexcept>

pMR::Target::Target(MPI_Comm const communicator, int const targetRank,
        unsigned const uniqueSendID, unsigned const uniqueRecvID,
        bool const self, bool const null, bool const loop)
    :   mCommunicator(communicator),
        mTarget(targetRank),
        mUniqueSendID(uniqueSendID),
        mUniqueRecvID(uniqueRecvID),
        mNull(null),
        mSelf(self),
        mLoop(loop)
{
    if(mNull && mSelf)
    {
        throw std::logic_error("pMR: Target specified as both null and self.");
    }
    if(mNull && mLoop)
    {
        throw std::logic_error("pMR: Target specified as both null and loop.");
    }
    if(mSelf && mLoop)
    {
        throw std::logic_error("pMR: Target specified as both self and loop.");
    }
}

pMR::Target::Target(MPI_Comm const communicator, int const targetRank,
        unsigned const uniqueSendID, unsigned const uniqueRecvID)
    :   mCommunicator(communicator),
        mTarget(targetRank),
        mUniqueSendID(uniqueSendID),
        mUniqueRecvID(uniqueRecvID)
{
    queryTarget();
}

bool pMR::Target::isNull() const
{
    return mNull;
}

bool pMR::Target::isSelf() const
{
    return mSelf;
}

bool pMR::Target::isLoop() const
{
    return mLoop;
}

int pMR::Target::getTargetRank() const
{
    return mTarget;
}

int pMR::Target::getUniqueSendID() const
{
    return mUniqueSendID;
}

int pMR::Target::getUniqueRecvID() const
{
    return mUniqueRecvID;
}

MPI_Comm pMR::Target::getMPICommunicator() const
{
    return mCommunicator;
}

void pMR::Target::queryTarget()
{
    // Check if target is null
    if(mTarget == MPI_PROC_NULL)
    {
        mNull = true;
    }

    // Check if target is self/loop
    int ownRank;
    if(MPI_Comm_rank(mCommunicator, &ownRank))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }

    if(ownRank == mTarget)
    {
        if(mUniqueSendID == mUniqueRecvID)
        {
            mLoop = true;
        }
        else
        {
            mSelf = true;
        }
    }
}
