//! @file communicator.hpp
//! @brief Public interface for Communicator.
//!
//! @author Peter Georg
#ifndef pMR_BACKENDS_MPI_COMMUNICATOR_H
#define pMR_BACKENDS_MPI_COMMUNICATOR_H

#include <memory>
#include <vector>
extern "C"
{
#include <mpi.h>
}
#include "target.hpp"

namespace pMR
{
    //! @brief Backend-agnostic Communicator.
    class Communicator
    {
        public:
            //! @brief Convert MPI Communicator to backend-agnostic
            //!     Communicator.
            //! @param communicator MPI Communicator
            Communicator(MPI_Comm const communicator);
            //! @brief Creates a cartesian Communicator with topoloy.size()
            //!     dimensions.
            //! @param communicator MPI Communicator to use as base for new
            //!     cartesian Communicator.
            //! @param topology Number of processes in each dimension. Special
            //!     value 0 indicates to auto detect a best fit for that
            //!     dimension.
            //! @param periodic Wether a dimension is periodic or not.
            Communicator(MPI_Comm const communicator,
                    std::vector<int> const &topology,
                    std::vector<int> const &periodic);
            ~Communicator() = default;
            //! @brief Get the number of dimensions of the communicator.
            //! @return Number of dimensions.
            int dimensions() const;
            //! @brief The overall size of the communicator.
            //! @return Overall size of the communicator.
            int size() const;
            //! @brief Number of processes in specified dimension.
            //! @param dimension Dimension.
            //! @return Number of processes.
            int size(int const dimension) const;
            //! @brief Check if specified dimension is perodic.
            //! @param dimension Dimension.
            //! @return true if dimension is periodic, false otherwise.
            bool isPeriodic(int const dimension) const;
            //! @brief Get own coordinate of specified dimension.
            //! @param dimension Dimension.
            //! @return Own coordinate.
            int coordinate(int const dimension) const;
            //! @brief Get the neighbor in specified dimension, direction and
            //!     displacement.
            //! @param dimension Dimension.
            //! @param displacement Direction (>0: upwards, <0: downwards) and
            //!     displacement.
            //! @return Target representing requested neighbor.
            //! @note In case of a non-circular shift the neighbor might be
            //!     null, check the target for further information.
            Target getNeighbor(int const dimension,
                    int const displacement) const;
            MPI_Comm get() const;
            //! @brief Get the communicator's topology.
            //! @return Vector of length dimensions containing the number of
            //!     processes in each dimension.
            std::vector<int> topology() const;
            //! @brief Get the communicator's topology periodicity.
            //! @return Vector of length dimensions containing 0 or 1
            //!     indicating wether dimensions is periodic or not.
            std::vector<int> periodic() const;
        private:
            MPI_Comm mCommunicator;
            int mSize;
            std::vector<int> mCoordinates;
            std::vector<int> mTopology;
            std::vector<int> mPeriodic;
            bool mCartesian = false;
    };
}
#endif // pMR_BACKENDS_MPI_COMMUNICATOR_H
