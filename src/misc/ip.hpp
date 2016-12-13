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

#ifndef pMR_MISC_IP_H
#define pMR_MISC_IP_H

#include <string>
extern "C" {
#include <ifaddrs.h>
}

namespace pMR
{
    class IP
    {
    public:
        IP();
        IP(IP const &) = delete;
        IP(IP &&) = delete;
        IP &operator=(IP const &) = delete;
        IP &operator=(IP &&) = delete;
        ~IP();
        std::string getIPv4(std::string const &interface) const;
        std::string getIPv6(std::string const &interface) const;

    private:
        struct ifaddrs *mInterfaces = nullptr;
        std::string getIP(char const *interface, int const family) const;
        std::string structToString(struct ifaddrs const *ifa) const;
    };
}
#endif // pMR_MISC_IP_H
