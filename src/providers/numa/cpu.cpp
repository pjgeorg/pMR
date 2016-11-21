//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "cpu.hpp"
#include <stdexcept>
extern "C"
{
#include <numa.h>
#include <sched.h>
}
#include "support.hpp"

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
    checkSupport();

    int node = numa_node_of_cpu(cpu);

    if(node == -1)
    {
        throw std::runtime_error("pMR: Unable to get NUMA Node ID of CPU.");
    }

    return node;
}
