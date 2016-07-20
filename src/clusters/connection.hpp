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

//! @file connection.hpp
//! @brief Public interface for connections.
//!     This file is only required to generate common connection documentation,
//!     so that documentation has not to be added to every cluster specific
//!     file.
//!
//! @author Peter Georg
#ifndef pMR_CONNECTION_H
#define pMR_CONNECTION_H

//! @brief pMR namespace.
namespace pMR
{
    class Target;

    //! @brief Point-to-Point Connection
    class Connection
    {
        public:
            //! @brief Establish a point-to-point connection with target.
            //! @param target Target
            //! @warning Uses blocking routines, hence the order processes
            //!     call this function for different targets is important.
            //!     E.g. even ranks left neighbor first, odd ranks right
            //!     neighbor first.
            //! @note Using one thread for each connection can avoid dead
            //!     locks.
            //! @warning Only one thread may call any function associated with
            //!     a connection at any time, i.e. calls to a connection
            //!     have to be serialized.
            Connection(Target const &target);
            Connection(const Connection&) = delete;
            Connection(Connection&&) = delete;
            Connection& operator=(const Connection&) = delete;
            Connection& operator=(Connection&&) = delete;
            ~Connection() = default;
    };
}
#endif // pMR_CONNECTION_H
