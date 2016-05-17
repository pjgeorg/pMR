#include "communicator.hpp"
#include <numeric>
#include <stdexcept>
#include "../../misc/singleton.hpp"
#include "threadsupport.hpp"

pMR::Communicator::Communicator(MPI_Comm const communicator)
    :   mCommunicator(communicator) 
{
    if(MPI_Comm_size(communicator, &mSize))
    {
        throw std::runtime_error(
                "pMR: Unable to get size of MPI communicator.");
    }

    int type;
    if(MPI_Topo_test(mCommunicator, &type))
    {
        throw std::runtime_error("pMR: Unable to test MPI topology.");
    }

    if(type == MPI_CART)
    {
        int dimensions;
        if(MPI_Cartdim_get(mCommunicator, &dimensions))
        {
            throw std::runtime_error("pMR: Unable to query MPI communicator "
                    "for number of dimensions.");
        }

        mCoordinates.resize(dimensions);
        mDimensions.resize(dimensions);
        mPeriodic.resize(dimensions);

        if(MPI_Cart_get(mCommunicator, dimensions, mDimensions.data(),
                    mPeriodic.data(), mCoordinates.data()))
        {
            throw std::runtime_error("pMR: Unable to query cartesian MPI "
                    "communicator for geometry.");
        }
    }
}

int pMR::Communicator::dimensions() const
{
    return mDimensions.size();
}

int pMR::Communicator::size() const
{
    return mSize;
}

int pMR::Communicator::size(int const dimension) const
{
    try
    {
        return mDimensions.at(dimension);

    }
    catch(const std::exception &e)
    {
        throw std::out_of_range("pMR: Communicator dimension.");
    }
}

bool pMR::Communicator::isPeriodic(int const dimension) const
{
    try
    {
        return static_cast<bool>(mPeriodic.at(dimension));
    }
    catch(const std::exception &e)
    {
        throw std::out_of_range("pMR: Communicator dimension.");
    }
}

int pMR::Communicator::coordinate(int const dimension) const
{
    try
    {
        return mCoordinates.at(dimension);
    }
    catch(const std::exception &e)
    {
        throw std::out_of_range("pMR: Communicator dimension.");
    }
}

pMR::Target pMR::Communicator::getNeighbor(
        int const dimension, int const displacement) const
{
    if(dimension >= dimensions())
    {
        throw std::out_of_range("pMR: Communicator dimension.");
    }

    int origin;
    if(MPI_Comm_rank(mCommunicator, &origin))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }

    if(displacement == 0)
    {
        // Loop
        return Target(mCommunicator, origin, 0, 0, false, false, true);
    }

    int source;
    int target;
    if(MPI_Cart_shift(mCommunicator, dimension, displacement, &source, &target))
    {
        throw std::runtime_error("pMR: Unable to shift MPI rank.");
    }

    // Check if target is null
    if(target == MPI_PROC_NULL)
    {
        // Null
        return Target(mCommunicator, target, 0, 0, true, false, false);
    }

    int uniqueSendID = +displacement + (dimension + 1) * 1e3;
    int uniqueRecvID = -displacement + (dimension + 1) * 1e3;

    // Check if target is self/loop
    if(origin == target)
    {
        // Self
        return Target(mCommunicator, target, uniqueSendID, uniqueRecvID,
                false, true, false);
    }

    return Target(mCommunicator, target, uniqueSendID, uniqueRecvID,
            false, false, false);
}

MPI_Comm pMR::Communicator::get() const
{
    return mCommunicator;
}
