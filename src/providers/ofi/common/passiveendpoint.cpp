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

#include "passiveendpoint.hpp"
#include <stdexcept>
extern "C" {
#include <rdma/fi_cm.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>
}

pMR::OFI::PassiveEndpoint::PassiveEndpoint(Fabric &fabric, Info &info)
{
    if(fi_passive_ep(fabric.get(), info.get(), &mPassiveEndpoint, &mContext))
    {
        throw std::runtime_error("pMR: Unable to open endpoint.");
    }
}

pMR::OFI::PassiveEndpoint::~PassiveEndpoint()
{
    if(mPassiveEndpoint)
    {
        fi_close(&mPassiveEndpoint->fid);
    }
}

fid_pep *pMR::OFI::PassiveEndpoint::get()
{
    return mPassiveEndpoint;
}

fid_pep const *pMR::OFI::PassiveEndpoint::get() const
{
    return mPassiveEndpoint;
}

void pMR::OFI::PassiveEndpoint::bind(EventQueue &queue)
{
    if(fi_pep_bind(mPassiveEndpoint, &queue.get()->fid, 0))
    {
        throw std::runtime_error("pMR: Unable to bind EQ to endpoint.");
    }
}

void pMR::OFI::PassiveEndpoint::listen()
{
    if(fi_listen(mPassiveEndpoint))
    {
        throw std::runtime_error(
            "pMR: Unable to set passive endpoint to listen.");
    }
}

std::vector<std::uint8_t> pMR::OFI::PassiveEndpoint::getAddress() const
{
    std::vector<std::uint8_t> address;
    std::size_t size = 0;

    if(fi_getname(&mPassiveEndpoint->fid, NULL, &size) != -FI_ETOOSMALL)
    {
        throw std::runtime_error("pMR: Unable to detect size of address.");
    }

    address.resize({size});

    if(fi_getname(&mPassiveEndpoint->fid, address.data(), &size))
    {
        throw std::runtime_error("pMR: Unable to get address.");
    }

    return address;
}
