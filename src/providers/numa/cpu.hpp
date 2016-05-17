#ifndef pMR_PROVIDERS_NUMA_CPU_H
#define pMR_PROVIDERS_NUMA_CPU_H

namespace pMR { namespace numa
{
    int getCPU();
    int getNode(int const cpu);
}}
#endif // pMR_PROVIDERS_NUMA_CPU_H
