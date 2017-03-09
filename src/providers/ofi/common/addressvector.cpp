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

#include "addressvector.hpp"
#include <stdexcept>
extern "C" {
#include <rdma/fi_domain.h>
}
#include "../ofi.hpp"

pMR::ofi::AddressVector::AddressVector(Domain &domain)
{
    fi_av_attr attr = {};
    attr.type = FI_AV_MAP;
    attr.count = {cAVCount};
    attr.ep_per_node = 0;
    attr.name = NULL;

    if(fi_av_open(domain.get(), &attr, &mAddressVector, &mContext))
    {
        throw std::runtime_error("pMR: Unable to open AV.");
    }
}

pMR::ofi::AddressVector::~AddressVector()
{
    if(mAddressVector)
    {
        fi_close(&mAddressVector->fid);
    }
}

fid_av *pMR::ofi::AddressVector::get()
{
    return mAddressVector;
}

fid_av const *pMR::ofi::AddressVector::get() const
{
    return mAddressVector;
}

fi_addr_t pMR::ofi::AddressVector::add(std::vector<std::uint8_t> const &addr)
{
    fi_addr_t lAddr = {};

    if(fi_av_insert(mAddressVector, addr.data(), 1, &lAddr, 0, &mContext) != 1)
    {
        throw std::runtime_error("pMR: Unable to insert address.");
    }

    return lAddr;
}

std::string pMR::ofi::AddressVector::addressString(
    std::vector<std::uint8_t> const &addr) const
{
    size_t size = 0;

    fi_av_straddr(mAddressVector, addr.data(), NULL, &size);

    std::vector<char> str(size);
    fi_av_straddr(mAddressVector, addr.data(), str.data(), &size);

    if(size != str.size())
    {
        throw std::runtime_error("pMR: Unable to get address string.");
    }

    return std::string(str.begin(), str.end());
}
