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

#include "deviceattributes.hpp"
#include <stdexcept>

pMR::verbs::DeviceAttributes::DeviceAttributes(Context &context)
{
    if(ibv_query_device(context.get(), &mDeviceAttributes))
    {
        throw std::runtime_error("pMR: Unable to query device.");
    }
}

std::uint64_t pMR::verbs::DeviceAttributes::getNodeGUID() const
{
    return {mDeviceAttributes.node_guid};
}

std::uint8_t pMR::verbs::DeviceAttributes::getPortCount() const
{
    return {mDeviceAttributes.phys_port_cnt};
}

std::uint64_t pMR::verbs::DeviceAttributes::getMaxMemoryRegionSize() const
{
    return {mDeviceAttributes.max_mr_size};
}
