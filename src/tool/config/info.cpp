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
#include <iostream>
#include "parameter.hpp"
#include "usage.hpp"

int main(int argc, char **argv)
{
    if(parameterExists(argv, argv + argc, "--usage") or
        parameterExists(argv, argv + argc, "--help"))
    {
        printUsage();
    }

    if(parameterExists(argv, argv + argc, "--version"))
    {
        std::cout << sVersion << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--cluster"))
    {
        std::cout << sCluster << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--backend"))
    {
        std::cout << sBackend << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--thread"))
    {
        std::cout << sThread << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--cxx"))
    {
        std::cout << sCXX << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--cxxflags"))
    {
        std::cout << sCXXFlags << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--libs"))
    {
        std::cout << sLibs << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--ldflags"))
    {
        std::cout << sLDFlags << std::endl;
    }
    if(parameterExists(argv, argv + argc, "--config"))
    {
        std::cout << sConfig << std::endl;
    }
}
