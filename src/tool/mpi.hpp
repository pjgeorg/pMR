#ifndef pMR_TOOL_MPI_H
#define pMR_TOOL_MPI_H

extern "C"
{
#include <mpi.h>
}
#include "../misc/print.hpp"

void init(int argc, char **argv);
void finalize(int exitCode = 0);
void abort(int exitCode = 0);

int getRank(MPI_Comm communicator);

template<typename ...Types>
void printMaster(Types const &...args)
{
    if(getRank(MPI_COMM_WORLD) == 0)
    {
        pMR::print(args...);
    }
}
#endif // pMR_TOOL_MPI_H
