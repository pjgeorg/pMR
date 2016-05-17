//! @file connection.hpp
//! @brief Public interface for connections.
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
            //! @brief Non-blocking routine to synchronize origin and target.
            //! @warning Any outstanding synchronization or data transfer,
            //!     associated with the same connection, has to be finished
            //!     before initializing a fence.
            void initFence();
            //! @brief Wait for initialized non-blocking initFence() to finish.
            void waitFence();
    };
}
#endif // pMR_CONNECTION_H
