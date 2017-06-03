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

#include "usage.hpp"
#include "mpi.hpp"

void printUsage()
{
    printMaster("pMR Benchmark Tool usage:");
    printMaster();
    printMaster("  Select benchmark:");
    printMaster("    --benchmark         exchange | allreduce");
    printMaster();
    printMaster("  Optional parameters:");
    printMaster("    --dim D             Set number of dimensions.");
    printMaster("    --geom x y z t      Set process topology.");
    printMaster("    --periodic x y z t  Set process topology.");
    printMaster(
        "    --active x y z t    Set active channels. Non-active channels "
        "exchange zero sized messages to sync.");
    printMaster("    --maxIter n         Set maximum iterations.");
    printMaster("    --overallSize s     Set maximum sent data.");
    printMaster("    --minMsgSize m      Set minimum message size.");
    printMaster("    --MaxMsgSize M      Set maximum message size.");
    printMaster(
        "    --deltaMsgSize d    Set message size delta factor in percent.");
    printMaster("    --minDeltaMsgSize m Set minimum delta for message size.");
    printMaster("    --maxDeltaMsgSize M Set minimum delta for message size.");
    printMaster("    --bufferedSend      Enable buffered send.");
    printMaster("    --bufferedRecv      Enable buffered receive.");
    printMaster("    --threaded          Enable threaded communication.");
    printMaster("    --verify            Enable communication verification.");
    printMaster("    --granularity g     Set AllReduce size granularity.");
    printMaster("    --bitExact          Use bit identical allreduce.");

    finalize();
}
