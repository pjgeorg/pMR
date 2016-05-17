#include "topology.hpp"
#include <stdexcept>
extern "C"
{
#include <scif.h>
}

std::uint16_t pMR::scif::getNodeID()
{
    uint16_t iD;
    if(!scif_get_nodeIDs(NULL, 0, &iD))
    {
        throw std::runtime_error("pMR: Unable to get Node ID.");
    }
    return iD;
}
