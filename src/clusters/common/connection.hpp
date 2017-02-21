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
//!
//! @mainpage
//! @htmlinclude redirect.html
#ifndef pMR_CONNECTION_H
#define pMR_CONNECTION_H

#include <memory>
#include "config.hpp"
#include "provider.hpp"

//! @brief pMR namespace.
namespace pMR
{
    class SendMemoryWindow;
    class RecvMemoryWindow;
    class Target;

    //! @brief Point-to-Point Connection
    class Connection
    {
        friend SendMemoryWindow;
        friend RecvMemoryWindow;

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
        Connection(Connection const &) = delete;
        Connection(Connection &&) = default;
        Connection &operator=(Connection const &) = delete;
        Connection &operator=(Connection &&) = default;
        ~Connection() = default;

    private:
        Provider mProvider;
        void connect(Target const &target);

#ifdef pMR_PROVIDER_CMA
        std::shared_ptr<cma::Connection> mCMA;
        void connectCMA(Target const &target);
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        std::shared_ptr<mpi::Connection> mMPI;
        void connectMPI(Target const &target);
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        std::shared_ptr<null::Connection> mNull;
        void connectNull(Target const &target);
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        std::shared_ptr<ofi::Connection> mOFI;
        void connectOFI(Target const &target, ofi::Info const &info);
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_SCIF
        std::shared_ptr<scif::Connection> mSCIF;
        void connectSCIF(Target const &target);
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        std::shared_ptr<self::Connection> mSelf;
        void connectSelf(Target const &target);
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        std::shared_ptr<verbs::Connection> mVerbs;
        void connectVerbs(Target const &target, verbs::Device const &device,
            std::uint8_t const portNumber = 1);
#endif // pMR_PROVIDER_VERBS
    };
}
#endif // pMR_CONNECTION_H
