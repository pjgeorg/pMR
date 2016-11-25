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

#include <cstdint>
#include <limits>
#include "config.hpp"

static_assert(std::numeric_limits<pMR::size_type>::is_integer,
        "size_type must be of integer type.");

#ifdef pMR_PROVIDER_CMA
static_assert(std::numeric_limits<pMR::size_type>::max()
        <= std::numeric_limits<std::size_t>::max(),
        "CMA max message size lower than desired pMR max message size.");
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
static_assert(std::numeric_limits<pMR::size_type>::max()
        <= std::numeric_limits<int>::max(),
        "MPI max message size lower than desired pMR max message size.");
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_OFI
static_assert(std::numeric_limits<pMR::size_type>::max()
        <= std::numeric_limits<std::size_t>::max(),
        "OFI max message size lower than desired pMR max message size.");
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_SELF
static_assert(std::numeric_limits<pMR::size_type>::max()
        <= std::numeric_limits<std::size_t>::max(),
        "self max message size lower than desired pMR max message size.");
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
static_assert(std::numeric_limits<pMR::size_type>::max()
        <= std::numeric_limits<std::uint32_t>::max(),
        "verbs max message size lower than desired pMR max message size.");
#endif // pMR_PROVIDER_VERBS
