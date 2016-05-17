#include "memoryregion.hpp"

pMR::verbs::ODP::ODP(Context& context)
{
    mHasODP = false;
}

int pMR::verbs::updateMemoryRegionAccessODP(int access)
{
    return access;
}
