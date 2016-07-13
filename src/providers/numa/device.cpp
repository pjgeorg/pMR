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

#include "topology.hpp"
#include <fstream>
#include <iterator>
#include <string>
#include <stdexcept>
#include "support.hpp"
#include "../../misc/singleton.hpp"

int pMR::numa::getNode(std::string const devicePath)
{
    Singleton<Support>::Instance();

    std::ifstream deviceFile(devicePath + "/device/numa_node", std::ios::in);
    if(!deviceFile)
    {
        throw std::runtime_error("pMR: Unable to open device file.");
    }

    return *(std::istream_iterator<int>(deviceFile));
}
