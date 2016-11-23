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

#include "context.hpp"
#include <stdexcept>
#include "deviceattributes.hpp"
#include "portattributes.hpp"
#include "../../misc/string.hpp"

pMR::verbs::Context::Context(Device const &device)
{
    mContext = ibv_open_device(const_cast<ibv_device*>(device.get()));
    if(!mContext)
    {
        throw std::runtime_error(toString("pMR: Could not open device",
                    device.getName(), "."));
    }

    DeviceAttributes deviceAttr(*this);

    mMaxMemoryRegionSize = {deviceAttr.getMaxMemoryRegionSize()};

    mMaxMessageSize = {PortAttributes(*this, 1).getMaxMessageSize()};

    for(auto port = deviceAttr.getPortCount(); port > 1; --port)
    {
        mMaxMessageSize = {std::min(mMaxMessageSize,
                PortAttributes(*this, port).getMaxMessageSize())};
    }
}

pMR::verbs::Context::~Context()
{
    ibv_close_device(mContext);
}

ibv_context* pMR::verbs::Context::get()
{
    return mContext;
}

ibv_context const* pMR::verbs::Context::get() const
{
    return mContext;
}

std::uint64_t pMR::verbs::Context::getMaxMemoryRegionSize() const
{
    return {mMaxMemoryRegionSize};
}

std::uint32_t pMR::verbs::Context::getMaxMessageSize() const
{
    return {mMaxMessageSize};
}
