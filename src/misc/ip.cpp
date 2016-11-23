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

#include "ip.hpp"
#include <vector>
#include <stdexcept>
#include <cstring>
extern "C"
{
#include <arpa/inet.h>
}

pMR::IP::IP()
{
    if(getifaddrs(&mInterfaces) == -1)
    {
        throw std::runtime_error(
                "pMR: Unable to get network interfaces description.");
    }
}

pMR::IP::~IP()
{
    if(mInterfaces)
    {
        freeifaddrs(mInterfaces);
    }
}

std::string pMR::IP::getIPv4(std::string const &interface) const
{
    return getIP(interface.c_str(), AF_INET);
}

std::string pMR::IP::getIPv6(std::string const &interface) const
{
    return getIP(interface.c_str(), AF_INET6);
}

std::string pMR::IP::getIP(char const *interface, int const family) const
{
    auto *ifa = mInterfaces;

    while(ifa)
    {
        if(ifa->ifa_addr->sa_family == family)
        {
            if(std::strcmp(ifa->ifa_name, interface) == 0)
            {
                return structToString(ifa);
            }
        }
        ifa = ifa->ifa_next;
    }
    throw std::runtime_error("pMR: Desired interface unavailable.");
}

std::string pMR::IP::structToString(struct ifaddrs const *ifa) const
{
    auto const family = ifa->ifa_addr->sa_family;

    void *src = nullptr;
    std::vector<char> ip;

    switch(family)
    {
        case AF_INET:
        {
            struct sockaddr_in *in = nullptr;
            in = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
            src = static_cast<void*>(&in->sin_addr);
            ip.resize(INET_ADDRSTRLEN);
            break;
        }
        case AF_INET6:
        {
            struct sockaddr_in6 *in6 = nullptr;
            in6 = reinterpret_cast<struct sockaddr_in6*>(ifa->ifa_addr);
            src = static_cast<void*>(&in6->sin6_addr);
            ip.resize(INET6_ADDRSTRLEN);
            break;
        }
        default:
        {
            throw std::runtime_error("pMR: Unknown Address Family.");
        }
    }

    if(inet_ntop(family, src, ip.data(), ip.size()) == NULL)
    {
        throw std::runtime_error(
                "pMR: Unable to convert network address structure to string.");
    }

    return ip.data();
}
