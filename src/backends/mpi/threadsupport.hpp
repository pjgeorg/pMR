#ifndef pMR_BACKENDS_MPI_THREADSUPPORT_H
#define pMR_BACKENDS_MPI_THREADSUPPORT_H

#include <string>
extern "C"
{
#include <mpi.h>
}

namespace pMR { namespace backend
{
    class ThreadSupport
    {
        public:
            ThreadSupport();
            ~ThreadSupport() = default;
            bool multiple() const;
            bool serialized() const;
        private:
            bool mMultiple = false;
            bool mSerialized = false;
    };
}}
#endif // pMR_BACKENDS_MPI_THREADSUPPORT_H
