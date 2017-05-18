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

#include "psm.hpp"
#include <stdexcept>
extern "C" {
#include <psm2.h>
#include <stdlib.h>
}
#include "error.hpp"
#include "psm2.hpp"

pMR::PSM2::PSM::PSM()
{
    setEnvironment();

    int vMajor = PSM2_VERNO_MAJOR;
    int vMinor = PSM2_VERNO_MINOR;

    auto err = psm2_error_register_handler(NULL, PSM2_ERRHANDLER_NO_HANDLER);
    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to register error handler: " + toString(err));
    }

    err = psm2_init(&vMajor, &vMinor);
    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to initialize PSM2: " + toString(err));
    }
}

pMR::PSM2::PSM::~PSM()
{
    psm2_finalize();
}

void pMR::PSM2::PSM::setEnvironment() const
{
    if(std::getenv(cLocalRankIDEnv) == NULL)
    {
        for(auto localRankIDEnv : cLocalRankIDEnvs)
        {
            auto rankID = std::getenv(localRankIDEnv);
            if(rankID)
            {
                setenv(cLocalRankIDEnv, rankID, 0);
                break;
            }
        }
    }

    if(std::getenv(cLocalNRanksEnv) == NULL)
    {
        for(auto localNRanksEnv : cLocalNRanksEnvs)
        {
            auto nRanks = std::getenv(localNRanksEnv);
            if(nRanks)
            {
                setenv(cLocalNRanksEnv, nRanks, 0);
                break;
            }
        }
    }
}
