#include "info.hpp"
#include <cstdlib>
#include "../misc/print.hpp"
#include "mpi.hpp"
#include "parameter.hpp"
#include "usage.hpp"
#include "benchmark.hpp"

int main(int argc, char **argv)
{
    try
    {
        init(argc, argv);
    
        if(parameterExists(argv, argv + argc, "--usage") or
            parameterExists(argv, argv + argc, "--help"))
        {
            printUsage();
        }
    
        // Run desired benchmark
        if(parameterExists(argv, argv + argc, "--benchmark"))
        {
            runBenchmark(argc, argv);
        }
    
        // Print requested information
        printInfo(argc, argv);

        finalize();
    }
    catch(const std::exception &e)
    {
        pMR::print(e.what());
        abort(1);
    }
}
