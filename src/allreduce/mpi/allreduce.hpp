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

#ifndef pMR_ALLREDUCE_MPI_ALLREDUCE_H
#define pMR_ALLREDUCE_MPI_ALLREDUCE_H

extern "C" {
#include <mpi.h>
}
#include "../../communicator.hpp"
#include "../../config.hpp"
#include "../../operations.hpp"

namespace pMR
{
    namespace MPI
    {
        class AllReduce
        {
        public:
            AllReduce(Communicator const &communicator, void *buffer,
                size_type const sizeByte);
            AllReduce(const AllReduce &) = delete;
            AllReduce(AllReduce &&) = default;
            AllReduce &operator=(const AllReduce &) = delete;
            AllReduce &operator=(AllReduce &&) = default;
            ~AllReduce() = default;

            template<typename T>
            bool execute(
                void *buffer, Operation const op, size_type const count) const;

            template<typename T>
            bool execute(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count) const;

            template<typename T>
            bool executeBit(
                void *buffer, Operation const op, size_type const count) const;

            template<typename T>
            bool executeBit(void *buffer,
                void (*op)(T const *in, T *inout, size_type const count),
                size_type const count) const;

        private:
            MPI_Comm const mCommunicator;
            int const mRank;

            MPI_Op convert(Operation const op) const;
            bool execute(void *buffer, Operation const op,
                MPI_Datatype const datatype, size_type const count) const;
            bool executeBit(void *buffer, Operation const op,
                MPI_Datatype const datatype, size_type const count) const;
        };

        template<>
        bool AllReduce::execute<int>(
            void *buffer, Operation const op, size_type const count) const;
        template<>
        bool AllReduce::executeBit<int>(
            void *buffer, Operation const op, size_type const count) const;

        template<>
        bool AllReduce::execute<float>(
            void *buffer, Operation const op, size_type const count) const;
        template<>
        bool AllReduce::executeBit<float>(
            void *buffer, Operation const op, size_type const count) const;

        template<>
        bool AllReduce::execute<double>(
            void *buffer, Operation const op, size_type const count) const;
        template<>
        bool AllReduce::executeBit<double>(
            void *buffer, Operation const op, size_type const count) const;
    }
}

template<typename T>
bool pMR::MPI::AllReduce::execute(
    void *buffer, Operation op, size_type const count) const
{
    return {false};
}

template<typename T>
bool pMR::MPI::AllReduce::execute(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count) const
{
    return {false};
}

template<typename T>
bool pMR::MPI::AllReduce::executeBit(
    void *buffer, Operation op, size_type const count) const
{
    return {false};
}

template<typename T>
bool pMR::MPI::AllReduce::executeBit(void *buffer,
    void (*op)(T const *in, T *inout, size_type const count),
    size_type const count) const
{
    return {false};
}
#endif // pMR_ALLREDUCE_MPI_ALLREDUCE_H
