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

#include "memoryregion.hpp"

pMR::verbs::ODP::ODP(Context& context)
{
    std::uint32_t const capMask = IBV_ODP_SUPPORT_SEND | IBV_ODP_SUPPORT_RECV;

    ibv_device_attr_ex extendedAttr;

    if(ibv_query_device_ex(context.get(), NULL, &extendedAttr))
    {
        throw std::runtime_error("pMR: Unable to (extended) query device.");
    }

    if(!(extendedAttr.odp_caps.general_caps & IBV_ODP_SUPPORT) ||
        (extendedAttr.odp_caps.per_transport_caps.rc_odp_caps & capMask)
        != capMask)
    {
        mHasODP = false;
    }
    else
    {
        mHasODP = true;
    }
}

int pMR::verbs::updateMemoryRegionAccessODP(int access)
{
    return access | IBV_ACCESS_ON_DEMAND;
}
