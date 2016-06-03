#include "mpi.hpp"
#include <cstdlib>
#include <stdexcept>

void init(int argc, char **argv)
{
    int required = MPI_THREAD_MULTIPLE;
    int provided;

    if(MPI_Init_thread(&argc, &argv, required, &provided))
    {
        throw std::runtime_error("pMR: Unable to initialize MPI.");
    }
}

void finalize(int exitCode)
{
    MPI_Finalize();
    std::exit(exitCode);
}

void abort(int exitCode)
{
    MPI_Abort(MPI_COMM_WORLD, exitCode);
    std::exit(exitCode);
}

int getRank(MPI_Comm communicator)
{
    int rank;
    if(MPI_Comm_rank(communicator, &rank))
    {
        throw std::runtime_error("pMR: Unable to determine own MPI rank.");
    }

    return rank;
}
