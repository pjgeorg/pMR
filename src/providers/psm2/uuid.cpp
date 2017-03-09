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

#include "uuid.hpp"
#include <cstdlib>
#include <stdexcept>
#include <string>
#include "psm2.hpp"

pMR::PSM2::UUID::UUID()
{
    auto charUUID = std::getenv(cUUIDEnv);

    auto stringUUID = [&]() {
        if(charUUID)
        {
            return std::string(charUUID);
        }
        else
        {
            return std::string(cDefaultUUID);
        }
    }();

    if(sscanf(stringUUID.c_str(),
           "%2hhx%2hhx%2hhx%2hhx-%2hhx%2hhx-%2hhx%2hhx-%2hhx%2hhx-"
           "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
           &mUUID[0], &mUUID[1], &mUUID[2], &mUUID[3], &mUUID[4], &mUUID[5],
           &mUUID[6], &mUUID[7], &mUUID[8], &mUUID[9], &mUUID[10], &mUUID[11],
           &mUUID[12], &mUUID[13], &mUUID[14], &mUUID[15]) != 16)
    {
        throw std::runtime_error("pMR: Wrong UUID format.");
    }
};
