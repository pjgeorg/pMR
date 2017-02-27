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
}
#include "error.hpp"

pMR::psm2::PSM::PSM()
{
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

pMR::psm2::PSM::~PSM()
{
    psm2_finalize();
}
