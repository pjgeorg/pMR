#include "info.hpp"
#include "parameter.hpp"
#include "mpi.hpp"

void printInfo(int argc, char **argv)
{
    if(parameterExists(argv, argv + argc, "--libs"))
    {
        printMaster(sLibs);
    }
    if(parameterExists(argv, argv + argc, "--ldflags"))
    {
        printMaster(sLDFlags);
    }
    if(parameterExists(argv, argv + argc, "--cxx"))
    {
        printMaster(sCXX);
    }
    if(parameterExists(argv, argv + argc, "--cxxflags"))
    {
        printMaster(sCXXFlags);
    }
    if(parameterExists(argv, argv + argc, "--cluster"))
    {
        printMaster(sCluster);
    }
    if(parameterExists(argv, argv + argc, "--backend"))
    {
        printMaster(sBackend);
    }
    if(parameterExists(argv, argv + argc, "--thread"))
    {
        printMaster(sThread);
    }
    if(parameterExists(argv, argv + argc, "--version"))
    {
        printMaster(sVersion);
    }
}
