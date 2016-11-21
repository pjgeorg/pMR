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
#include "node.hpp"

#ifdef QPACE2_WARN_TOPOLOGY
#   include "../../misc/print.hpp"
#endif // QPACE2_WARN TOPOLOGY

std::uint8_t pMR::detectBestPort(Node const &origin, Node const &target)
{
    // Origin and Target on same brick
    if(origin.getNodeGUID() == target.getNodeGUID())
    {
        if(origin.getSCIFNodeID() + target.getSCIFNodeID() != 5)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    // Origin and Target in same hyperblock
    if(origin.getSwitchLID(1) == target.getSwitchLID(1) &&
            origin.getSwitchLID(2) == target.getSwitchLID(2))
    {
        if(origin.getSCIFNodeID() + target.getSCIFNodeID() < 6)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    // At least one common switch
    for(std::uint8_t portNumber = 1; portNumber != 3; ++portNumber)
    {
        if(origin.getSwitchLID(portNumber) == target.getSwitchLID(portNumber))
        {
            return portNumber;
        }
    }

    // No common switch
#ifdef QPACE2_WARN_TOPOLOGY
    print("pMR: Using bad path. Check topology.");
#endif // QPACE2_WARN TOPOLOGY

    if(origin.getSCIFNodeID() + target.getSCIFNodeID() < 6)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
