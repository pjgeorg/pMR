#ifndef pMR_PROVIDERS_NUMA_TOPOLOGY_H
#define pMR_PROVIDERS_NUMA_TOPOLOGY_H

#include <string>

namespace pMR { namespace numa
{
    int getNode(std::string const devicePath);

    // Get NUMA Node ID of current process
    int getNode();
}}
#endif // pMR_PROVIDERS_NUMA_TOPOLOGY_H
