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
        mTopology.resize(dimensions);
        mPeriodic.resize(dimensions);

        if(MPI_Cart_get(mCommunicator, dimensions, mTopology.data(),
                    mPeriodic.data(), mCoordinates.data()))
        {
            throw std::runtime_error("pMR: Unable to query cartesian MPI "
                    "communicator for geometry.");
        }
    }
}

pMR::Communicator::Communicator(MPI_Comm const communicator,
        std::vector<int> const &topology, std::vector<int> const &periodic)
{
    mTopology = topology;
    mPeriodic = periodic;
    mCoordinates.resize(mTopology.size());

    // Check dimensions
    if(mTopology.size() != mPeriodic.size())
    {
        throw std::runtime_error(
                "pMR: Number of desired dimensions can not be determined.");
    }

    // Get size of communicator
    if(MPI_Comm_size(communicator, &mSize))
    {
        throw std::runtime_error(
                "pMR: Unable to get size of MPI communicator.");
    }

    // Calculate size of cartesian communicator
    if(MPI_Dims_create(mSize, mTopology.size(), mTopology.data()))
    {
        throw std::runtime_error(
                "pMR: Unable to calculate size of cartesian communicator.");
    }

    // Create cartesian communicator
    if(MPI_Cart_create(communicator, mTopology.size(), mTopology.data(),
                mPeriodic.data(), 0, &mCommunicator))
    {
        throw std::runtime_error(
                "pMR: Unable to create cartesian communicator.");
    }
    if(mCommunicator == MPI_COMM_NULL)
    {
        throw std::runtime_error(
                "pMR: Unable to create cartesian communicator.");
    }
    mCartesian = true;

    // Get own coordinates
    int rank;
    if(MPI_Comm_rank(mCommunicator, &rank))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }
    if(MPI_Cart_coords(mCommunicator, rank, mTopology.size(),
                mCoordinates.data()))
    {
        throw std::runtime_error("pMR: Unable to determine own coordinates.");
    }
}

int pMR::Communicator::dimensions() const
{
    return mTopology.size();
}

int pMR::Communicator::size() const
{
    return mSize;
}

int pMR::Communicator::size(int const dimension) const
{
    try
    {
        return mTopology.at(dimension);

    }
    catch(const std::exception &e)
    {
        throw std::out_of_range("pMR: Communicator dimension out of range.");
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
        throw std::out_of_range("pMR: Communicator dimension out of range.");
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
        throw std::out_of_range("pMR: Communicator dimension out of range.");
    }
}

pMR::Target pMR::Communicator::getNeighbor(
        int const dimension, int const displacement) const
{
    if(dimension >= dimensions())
    {
        throw std::out_of_range("pMR: Communicator dimension out of range.");
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

    // Check if target is self
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

std::vector<int> pMR::Communicator::topology() const
{
    return mTopology;
}

std::vector<int> pMR::Communicator::periodic() const
{
    return mPeriodic;
}
