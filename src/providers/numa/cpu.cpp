#include "cpu.hpp"
#include <stdexcept>
extern "C"
{
#include <numa.h>
#include <sched.h>
}
#include "support.hpp"
#include "../../misc/singleton.hpp"

int pMR::numa::getCPU()
{
    int cpu = sched_getcpu();

    if(cpu == -1)
    {
        throw std::runtime_error("pMR: Unable to get CPU of current process.");
    }

    return cpu;
}

int pMR::numa::getNode(int const cpu)
{
    Singleton<Support>::Instance();

    int node = numa_node_of_cpu(cpu);

    if(node == -1)
    {
        throw std::runtime_error("pMR: Unable to get NUMA Node ID of CPU.");
    }

    return node;
}
