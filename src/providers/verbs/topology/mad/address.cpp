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

#include "address.hpp"

pMR::verbs::mad::Address::Address(std::uint8_t const portNumber)
{
    mAddress.is_global = 0;
    mAddress.src_path_bits = 0;
    mAddress.port_num = portNumber;
}

ibv_ah_attr* pMR::verbs::mad::Address::get()
{
    return &mAddress;
}

ibv_ah_attr const* pMR::verbs::mad::Address::get() const
{
    return &mAddress;
}

pMR::verbs::mad::SubnetManager::SubnetManager(PortAttributes &portAttributes,
        std::uint8_t const portNumber)
    :   Address(portNumber)
{
    mAddress.dlid = portAttributes.getSubnetManagerLID();
    mAddress.sl = portAttributes.getSubnetManagerServiceLevel();
}
