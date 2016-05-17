#include "threadsupport.hpp"
#include <stdexcept>

pMR::backend::ThreadSupport::ThreadSupport()
{
    int provided;
    if(MPI_Query_thread(&provided) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to query MPI Thread support.");
    }

    if(provided == MPI_THREAD_MULTIPLE)
    {
        mMultiple = true;
    }

    if(provided == MPI_THREAD_SERIALIZED)
    {
        mSerialized = true;
    }
}

bool pMR::backend::ThreadSupport::multiple() const
{
    return mMultiple;
}

bool pMR::backend::ThreadSupport::serialized() const
{
    return mSerialized;
}
