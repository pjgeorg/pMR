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

#include "info.hpp"
#include "mpi.hpp"
#include "parameter.hpp"

void printInfo(int argc, char **argv)
{
    if(parameterExists(argv, argv + argc, "--version"))
    {
        printMaster(sVersion);
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
    if(parameterExists(argv, argv + argc, "--cxx"))
    {
        printMaster(sCXX);
    }
    if(parameterExists(argv, argv + argc, "--cxxflags"))
    {
        printMaster(sCXXFlags);
    }
    if(parameterExists(argv, argv + argc, "--libs"))
    {
        printMaster(sLibs);
    }
    if(parameterExists(argv, argv + argc, "--ldflags"))
    {
        printMaster(sLDFlags);
    }
    if(parameterExists(argv, argv + argc, "--config"))
    {
        printMaster(sConfig);
    }
}
