//! @file target.hpp
//! @brief Public interface for Target.
//!
//! @author Peter Georg
#ifndef pMR_BACKENDS_MPI_TARGET_H
#define pMR_BACKENDS_MPI_TARGET_H

#include <memory>
extern "C"
{
#include <mpi.h>
}

namespace pMR
{
    //! @brief Backend-agnostic Target.
    class Target
    {
        public:
            //! @brief Convert MPI Target to backend-agnostic Target.
            //! @warning Only one of the paramteter null, self and loop
            //!     can be true.
            //! @param communicator MPI Communicator.
            //! @param targetRank Target MPI rank.
            //! @param uniqueSendID Unique ID to distiguish MPI messages
            //!     (Send Tag).
            //! @param uniqueRecvID Unique ID to distinguish MPI messages
            //!     (Recv Tag).
            //! @param null Target is no communication (MPI_PROC_NULL).
            //! @param self Target is self communication.
            //! @param loop Target is loop communication.
            //! @return Return backend-agnostic Target.
            Target(MPI_Comm const communicator, int const targetRank,
                    unsigned const uniqueSendID, unsigned const uniqueRecvID,
                    bool const null, bool const self, bool const loop);
            //! @brief Convert MPI Target to backend-agnostic Target.
            //! @param communicator MPI Communicator.
            //! @param targetRank Target MPI rank.
            //! @param uniqueSendID Unique ID to distiguish MPI messages
            //!     (Send Tag).
            //! @param uniqueRecvID Unique ID to distinguish MPI messages
            //!     (Recv Tag).
            //! @return Return backend-agnostic Target.
            Target(MPI_Comm const communicator, int const targetRank,
                    unsigned const uniqueSendID, unsigned const uniqueRecvID);
            ~Target() = default;
            //! @brief Checks whether the target is null (MPI_PROC_NULL).
            //! @return true if null, false otherwise.
            bool isNull() const;
            //! @brief Checks whether the target is the same process and self.
            //! @details Connections to the same process can be of two
            //!     different types: Self and Loop.
            //!     Self means that there is another connection attempt by the
            //!     same process - RecvWindows are linked to the other
            //!     connections's SendWindow and vice versa.
            //! @note A self connection to the same process can be created by
            //!     explicitely specifying at Target creation, creating a Target
            //!     with unique IDs that are sufficient to distinguish the
            //!     connections, or by using Communicator::getNeighbor() with a
            //!     displacement other than 0.
            //! @return true if self, false otherwise.
            bool isSelf() const;
            //! @brief Checks whether the target is the same process and loop.
            //! @details Connections to the same process can be of two
            //!     different types: Self and Loop.
            //!     Loop means that RecvWindows associated with the connection
            //!     are linked with SendWindows of the same connection. No other
            //!     connection is involved.
            //! @note A loop to the same process can be created by explicitely
            //!     specifying at Target creation, creating a Target with unique
            //!     IDs that are not sufficient to distinguish the connections,
            //!     or by using Communicator::getNeighbor() with a displacement
            //!     of 0.
            //! @return true if loop, false otherwise.
            bool isLoop() const;
            int getTargetRank() const;
            int getUniqueSendID() const;
            int getUniqueRecvID() const;
            MPI_Comm getMPICommunicator() const;
        private:
            MPI_Comm const mCommunicator;
            int const mTarget;
            unsigned const mUniqueSendID;
            unsigned const mUniqueRecvID;
            bool mNull = false;
            bool mSelf = false;
            bool mLoop = false;
            void queryTarget();
    };
}
#endif // pMR_BACKENDS_MPI_TARGET_H
