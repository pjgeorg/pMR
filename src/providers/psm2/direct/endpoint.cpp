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

#include "endpoint.hpp"
#include <stdexcept>
#include "../config.hpp"
#include "../error.hpp"
#include "options.hpp"
#include "uuid.hpp"

pMR::psm2::Endpoint::Endpoint()
{
    Options options;
    options.setAffinity(PSM2_EP_OPEN_AFFINITY_SKIP);

    auto err =
        psm2_ep_open(UUID().get(), options.get(), &mEndpoint, &mEndpointID);

    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to open Endpoint: " + toString(err));
    }
}

pMR::psm2::Endpoint::~Endpoint()
{
    if(psm2_ep_close(
           mEndpoint, PSM2_EP_CLOSE_GRACEFUL, PSMEndpointCloseTimeout))
    {
        psm2_ep_close(mEndpoint, PSM2_EP_CLOSE_FORCE, 0);
    }
}

psm2_ep_t pMR::psm2::Endpoint::get() const
{
    return mEndpoint;
}

std::uint64_t pMR::psm2::Endpoint::getNetworkID()
{
    return {psm2_epid_nid(mEndpointID)};
}

std::uint64_t pMR::psm2::Endpoint::getContext()
{
    return {psm2_epid_context(mEndpointID)};
}

psm2_epid_t pMR::psm2::Endpoint::getID() const
{
    return mEndpointID;
}

psm2_epaddr_t pMR::psm2::Endpoint::connect(psm2_epid_t const remoteEndpointID)
{
    psm2_epconn_t endpointConnection;
    if(psm2_ep_epid_lookup(remoteEndpointID, &endpointConnection) == PSM2_OK)
    {
        return endpointConnection.addr;
    }
    else
    {
        auto error = PSM2_OK;
        psm2_epaddr_t remoteEndpointAddress;

        psm2_ep_connect(mEndpoint, 1, &remoteEndpointID, NULL, &error,
            &remoteEndpointAddress, 0);

        if(error)
        {
            throw std::runtime_error(
                "Unable to connect to remote Endpoint: " + toString(error));
        }

        return remoteEndpointAddress;
    }
}
