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

#include "adapter.hpp"
#include <stdexcept>

int pMR::verbs::getAdapter(Devices const &devices, ibv_node_type const nodeType,
    ibv_transport_type const transportType, int deviceNumber)
{
    for(decltype(devices.size()) i = 0; i != devices.size(); ++i)
    {
        if(devices[i].getNodeType() == nodeType &&
            devices[i].getTransportType() == transportType)
        {
            if(--deviceNumber == 0)
            {
                return i;
            }
        }
    }

    return -1;
}

pMR::verbs::Device pMR::verbs::getIBAdapter(int const deviceNumber)
{
    return getIBAdapter(Devices(), deviceNumber);
}

pMR::verbs::Device pMR::verbs::getIBAdapter(
    Devices const &devices, int const deviceNumber)
{
    auto device = getAdapter(devices, {IBV_NODE_CA}, {IBV_TRANSPORT_IB});

    if(device == -1)
    {
        throw std::runtime_error("pMR: No Infiniband adapter found.");
    }

    return devices.at({device});
}
