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
#include "parameter.hpp"
#include "mpi.hpp"

void printUsage()
{
    printMaster("pMR Tool usage:");
    printMaster();
    printMaster("Supported operations: Info and Benchmark");
    printMaster();
    printMaster("Info:");
    printMaster("  Use to get information about pMR compilation.");
    printMaster("  Available parameters:");
    printMaster("    --version           Get version.");
    printMaster("    --cluster           Get configured cluster.");
    printMaster("    --backend           Get configured backend.");
    printMaster("    --thread            Get configured thread.");
    printMaster("    --cxx               Get CXX compiler.");
    printMaster("    --cxxflags          Get CXX compiler flags.");
    printMaster("    --libs              Get required libraries.");
    printMaster("    --ldflags           Get required LDFLAGS.");
    printMaster();
    printMaster();
    printMaster("Benchmark:");
    printMaster("  Use to run simple benchmarks.");
    printMaster("  Select benchmark:");
    printMaster("    --benchmark         exchange | sendrecv");
    printMaster();
    printMaster("  Optional parameters:");
    printMaster("    --dim D             Set number of dimensions.");
    printMaster("    --geom x y z t      Set process topology.");
    printMaster("    --periodic x y z t  Set process topology.");
    printMaster("    --maxIter n         Set maximum iterations.");
    printMaster("    --overallSize s     Set maximum sent data.");
    printMaster("    --minMsgSize m      Set minimum message size.");
    printMaster("    --MaxMsgSize M      Set maximum message size.");
    printMaster("    --deltaMsgSize d    Set message size delta.");
    printMaster("    --bufferedSend      Enable buffered send.");
    printMaster("    --bufferedRecv      Enable buffered receive.");

    finalize();
}
