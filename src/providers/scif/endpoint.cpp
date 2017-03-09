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
#include <cerrno>
#include <cstring>
#include <limits>
#include <stdexcept>
extern "C" {
#include <fcntl.h>
}
#include "../../misc/print.hpp"
#include "../../misc/string.hpp"

pMR::SCIF::Endpoint::Endpoint() : mEndpoint(scif_open())
{
    if(mEndpoint == static_cast<scif_epd_t>(-1))
    {
        throw std::runtime_error(
            toString("pMR: Unable to open endpoint:", std::strerror(errno)));
    }
}

pMR::SCIF::Endpoint::~Endpoint()
{
    if(scif_close(mEndpoint) == -1)
    {
        print("pMR: Probably unable to close endpoint:", std::strerror(errno));
    }
}

scif_epd_t &pMR::SCIF::Endpoint::get()
{
    return {mEndpoint};
}

scif_epd_t const &pMR::SCIF::Endpoint::get() const
{
    return {mEndpoint};
}

pMR::SCIF::Address pMR::SCIF::Endpoint::bind(std::uint16_t const port)
{
    auto boundPort = scif_bind(mEndpoint, port);

    if(boundPort < 0 || boundPort > std::numeric_limits<std::uint16_t>::max())
    {
        throw std::runtime_error(
            toString("pMR: Unable to bind endpoint:", std::strerror(errno)));
    }

    return Address({static_cast<std::uint16_t>(boundPort)});
}

void pMR::SCIF::Endpoint::listen()
{
    if(scif_listen(mEndpoint, 1) == -1)
    {
        throw std::runtime_error(
            toString("pMR: Unable to mark endpoint as listening:",
                std::strerror(errno)));
    }
}

bool pMR::SCIF::Endpoint::connect(Address &address, bool blocking)
{
    setFileDescriptorBlocking(blocking);

    errno = 0;
    if(scif_connect(mEndpoint, address.get()) == -1)
    {
        if(errno == EINPROGRESS)
        {
            return {false};
        }
        else
        {
            throw std::runtime_error(
                toString("pMR: Unable to connect to remote endpoint:",
                    std::strerror(errno)));
        }
    }
    else
    {
        return {true};
    }
}

pMR::SCIF::PeerEndpoint pMR::SCIF::Endpoint::accept(
    Address const &address, bool const blocking)
{
    Address peerAddress;
    scif_epd_t peerEndpoint;

    int flag = blocking ? SCIF_ACCEPT_SYNC : 0;

    errno = 0;
    if(scif_accept(mEndpoint, peerAddress.get(), &peerEndpoint, flag) == -1)
    {
        if(errno == EAGAIN)
        {
            return PeerEndpoint(-1);
        }
        else
        {
            throw std::runtime_error(
                toString("pMR: Unable to accept endpoint connection:",
                    std::strerror(errno)));
        }
    }
    else
    {
        if(peerAddress != address)
        {
            throw std::runtime_error(
                "pMR: Connection request by unknown endpoint.");
        }

        return PeerEndpoint(peerEndpoint);
    }
}

void pMR::SCIF::Endpoint::setFileDescriptorBlocking(bool const blocking)
{
    auto fd = scif_get_fd(mEndpoint);

    auto gFlags = fcntl(fd, F_GETFL, 0);
    if(gFlags < 0)
    {
        throw std::runtime_error("pMR: Unable to get current FD flags.");
    }
    auto sFlags = blocking ? (gFlags & ~O_NONBLOCK) : (gFlags | O_NONBLOCK);

    fcntl(fd, F_SETFL, sFlags);
}
