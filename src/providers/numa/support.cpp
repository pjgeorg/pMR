#include "support.hpp"
#include <stdexcept>
extern "C"
{
#include <numa.h>
}

pMR::numa::Support::Support()
{
    if(numa_available() == -1)
    {
        throw std::runtime_error("pMR: Support support unavailable.");
    }
}
