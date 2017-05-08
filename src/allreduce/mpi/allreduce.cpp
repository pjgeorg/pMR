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

#include "allreduce.hpp"

pMR::MPI::AllReduce::AllReduce(
    Communicator const &communicator, void *buffer, size_type const sizeByte)
    : mCommunicator(communicator.get()), mRank{communicator.ID()}
{
}

MPI_Op pMR::MPI::AllReduce::convert(Operation const op) const
{
    switch(op)
    {
        case Operation::Max:
            return MPI_MAX;
            break;
        case Operation::Min:
            return MPI_MIN;
            break;
        case Operation::Sum:
            return MPI_SUM;
            break;
        case Operation::Prod:
            return MPI_PROD;
            break;
        default:
            return MPI_NO_OP;
            break;
    }
}

bool pMR::MPI::AllReduce::execute(void *buffer, Operation const op,
    MPI_Datatype const datatype, size_type const count) const
{
    MPI_Allreduce(
        MPI_IN_PLACE, buffer, count, datatype, convert(op), mCommunicator);

    return {true};
}

bool pMR::MPI::AllReduce::executeBit(void *buffer, Operation const op,
    MPI_Datatype const datatype, size_type const count) const
{
    if(mRank == 0)
    {
        MPI_Reduce(MPI_IN_PLACE, buffer, count, datatype, convert(op), 0,
            mCommunicator);
    }
    else
    {
        MPI_Reduce(
            buffer, buffer, count, datatype, convert(op), 0, mCommunicator);
    }
    MPI_Bcast(buffer, count, datatype, 0, mCommunicator);

    return {true};
}

template<>
bool pMR::MPI::AllReduce::execute<int>(
    void *buffer, Operation op, size_type const count) const
{
    return {execute(buffer, op, MPI_INT, count)};
}

template<>
bool pMR::MPI::AllReduce::executeBit<int>(
    void *buffer, Operation op, size_type const count) const
{
    return {executeBit(buffer, op, MPI_INT, count)};
}

template<>
bool pMR::MPI::AllReduce::execute<float>(
    void *buffer, Operation op, size_type const count) const
{
    return {execute(buffer, op, MPI_FLOAT, count)};
}

template<>
bool pMR::MPI::AllReduce::executeBit<float>(
    void *buffer, Operation op, size_type const count) const
{
    return {executeBit(buffer, op, MPI_FLOAT, count)};
}

template<>
bool pMR::MPI::AllReduce::execute<double>(
    void *buffer, Operation op, size_type const count) const
{
    return {execute(buffer, op, MPI_DOUBLE, count)};
}

template<>
bool pMR::MPI::AllReduce::executeBit<double>(
    void *buffer, Operation op, size_type const count) const
{
    return {executeBit(buffer, op, MPI_DOUBLE, count)};
}
