#include "topology.hpp"
#include "cpu.hpp"

int pMR::numa::getNode()
{
    return getNode(getCPU());
}
