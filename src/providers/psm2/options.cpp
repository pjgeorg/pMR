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

#include "options.hpp"
#include <stdexcept>
#include "error.hpp"

pMR::PSM2::Options::Options()
{
    auto err = psm2_ep_open_opts_get_defaults(&mOptions);
    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to get default options: " + toString(err));
    }
}

psm2_ep_open_opts *pMR::PSM2::Options::get()
{
    return &mOptions;
}

psm2_ep_open_opts const *pMR::PSM2::Options::get() const
{
    return &mOptions;
}

void pMR::PSM2::Options::setTimeout(std::int64_t const timeout)
{
    mOptions.timeout = {timeout};
}

void pMR::PSM2::Options::setUnit(int const unit)
{
    mOptions.unit = {unit};
}

void pMR::PSM2::Options::setPort(int const port)
{
    mOptions.port = {port};
}

void pMR::PSM2::Options::setAffinity(int const affinity)
{
    mOptions.affinity = {affinity};
}
