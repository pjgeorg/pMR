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

#include "id.hpp"
#include <stdexcept>
extern "C"
{
#include <scif.h>
}

std::uint16_t pMR::scif::getNodeID()
{
    uint16_t iD;
    if(!scif_get_nodeIDs(NULL, 0, &iD))
    {
        throw std::runtime_error("pMR: Unable to get Node ID.");
    }
    return {iD};
}
