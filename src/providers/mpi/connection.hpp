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

#ifndef pMR_PROVIDERS_MPI_CONNECTION_H
#define pMR_PROVIDERS_MPI_CONNECTION_H

extern "C" {
#include <mpi.h>
}
#include <config.hpp>

namespace pMR
{
    class Target;

    namespace mpi
    {
        class Connection
        {
        public:
            Connection(Target const &target);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;
            MPI_Comm getCommunicator() const;
            int getTargetRank() const;
            int getSendTag() const;
            int getRecvTag() const;
            enum ThreadLevel getThreadLevel() const;

        private:
            MPI_Comm const mCommunicator = MPI_COMM_NULL;
            int const mTargetRank = MPI_PROC_NULL;
            int mSendTag = -1;
            int mRecvTag = -1;
            enum ThreadLevel mThreadLevel = ThreadLevel::Unknown;
        };
    }
}
#endif // pMR_PROVIDERS_MPI_CONNECTION_H
