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

#include "time.hpp"
#include <stdexcept>

double pMR::getTimeInSeconds()
{
    timespec timeSpec;
    if(clock_gettime(CLOCK_MONOTONIC, &timeSpec))
    {
        throw std::runtime_error("Unable to get current time.");
    }
    return static_cast<double>(timeSpec.tv_nsec)/1e9 + timeSpec.tv_sec;
}
