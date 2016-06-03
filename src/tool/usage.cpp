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
