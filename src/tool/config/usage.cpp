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
#include <iostream>

void printUsage()
{
    std::cout << "pMR Config Tool usage:" << std::endl;
    std::cout << std::endl;
    std::cout << "Info:" << std::endl;
    std::cout << "  Use to get information about pMR compilation." << std::endl;
    std::cout << "  Available parameters:" << std::endl;
    std::cout << "    --version           Get version." << std::endl;
    std::cout << "    --cluster           Get configured cluster." << std::endl;
    std::cout << "    --backend           Get configured backend." << std::endl;
    std::cout << "    --thread            Get configured thread." << std::endl;
    std::cout << "    --cxx               Get CXX compiler." << std::endl;
    std::cout << "    --cxxflags          Get CXX compiler flags." << std::endl;
    std::cout << "    --libs              Get required libraries." << std::endl;
    std::cout << "    --ldflags           Get required LDFLAGS." << std::endl;
    std::cout << "    --config            Get altered configuration." << std::endl;
}
