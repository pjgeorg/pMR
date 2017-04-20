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

#include "communicator.hpp"
#include <numeric>
#include <stdexcept>
#include "../../misc/singleton.hpp"
#include "threadsupport.hpp"

pMR::Communicator::Communicator(MPI_Comm const communicator)
    : mCommunicator{communicator}
{
    if(MPI_Comm_size(communicator, &mSize))
    {
        throw std::runtime_error(
            "pMR: Unable to get size of MPI communicator.");
    }

    // Get own ID
    if(MPI_Comm_rank(mCommunicator, &mID))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
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
            throw std::runtime_error(
                "pMR: Unable to query MPI communicator for number of "
                "dimensions.");
        }

        mCoordinates.resize({static_cast<unsigned>(dimensions)});
        mTopology.resize({static_cast<unsigned>(dimensions)});
        mPeriodic.resize({static_cast<unsigned>(dimensions)});

        if(MPI_Cart_get(mCommunicator, {dimensions}, mTopology.data(),
               mPeriodic.data(), mCoordinates.data()))
        {
            throw std::runtime_error(
                "pMR: Unable to query cartesian MPI communicator for "
                "geometry.");
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
    if(MPI_Dims_create(
           {mSize}, {static_cast<int>(mTopology.size())}, mTopology.data()))
    {
        throw std::runtime_error(
            "pMR: Unable to calculate size of cartesian communicator.");
    }

    // Create cartesian communicator
    if(MPI_Cart_create({communicator}, {static_cast<int>(mTopology.size())},
           mTopology.data(), mPeriodic.data(), 0, &mCommunicator))
    {
        throw std::runtime_error(
            "pMR: Unable to create cartesian communicator.");
    }
    if(mCommunicator == MPI_COMM_NULL)
    {
        throw std::runtime_error(
            "pMR: Unable to create cartesian communicator.");
    }

    // Get own ID
    if(MPI_Comm_rank(mCommunicator, &mID))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }

    // Get own coordinates
    if(MPI_Cart_coords(mCommunicator, {mID},
           {static_cast<int>(mTopology.size())}, mCoordinates.data()))
    {
        throw std::runtime_error("pMR: Unable to determine own coordinates.");
    }
}

int pMR::Communicator::dimensions() const
{
    return {static_cast<int>(mTopology.size())};
}

int pMR::Communicator::size() const
{
    return {mSize};
}

int pMR::Communicator::size(int const dimension) const
{
    try
    {
        return {mTopology.at(dimension)};
    }
    catch(std::exception const &e)
    {
        throw std::out_of_range("pMR: Communicator dimension out of range.");
    }
}

bool pMR::Communicator::isPeriodic(int const dimension) const
{
    try
    {
        return {static_cast<bool>(mPeriodic.at(dimension))};
    }
    catch(std::exception const &e)
    {
        throw std::out_of_range("pMR: Communicator dimension out of range.");
    }
}

int pMR::Communicator::ID() const
{
    return {mID};
}

int pMR::Communicator::coordinate(int const dimension) const
{
    try
    {
        return {mCoordinates.at(dimension)};
    }
    catch(std::exception const &e)
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

    int source;
    int target;
    if(MPI_Cart_shift(
           mCommunicator, {dimension}, {displacement}, &source, &target))
    {
        throw std::runtime_error("pMR: Unable to shift MPI rank.");
    }

    if(target == MPI_PROC_NULL)
    {
        return Target({mCommunicator}, {target}, 0, 0, {true}, {false});
    }

    auto uniqueSendID = +displacement + (dimension + 1) * 1000;
    auto uniqueRecvID = -displacement + (dimension + 1) * 1000;

    if(mID == target)
    {
        return Target({mCommunicator}, {target}, {uniqueSendID}, {uniqueRecvID},
            {false}, {true});
    }

    return Target({mCommunicator}, {target}, {uniqueSendID}, {uniqueRecvID},
        {false}, {false});
}

pMR::Target pMR::Communicator::getTarget(int const ID) const
{
    if(ID < 0)
    {
        return Target({mCommunicator}, {MPI_PROC_NULL}, 0, 0, {true}, {false});
    }

    if(ID == mID)
    {
        return Target({mCommunicator}, {mID}, 0, 0, {false}, {true});
    }

    if(ID >= mSize)
    {
        throw std::runtime_error("pMR: Target ID out of range.");
    }

    return Target({mCommunicator}, {ID}, {ID}, {mID}, {false}, {false});
}

MPI_Comm pMR::Communicator::get() const
{
    return {mCommunicator};
}

std::vector<int> pMR::Communicator::topology() const
{
    return mTopology;
}

std::vector<int> pMR::Communicator::periodic() const
{
    return mPeriodic;
}
