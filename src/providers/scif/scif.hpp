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

#ifndef pMR_PROVIDERS_SCIF_SCIF_H
#define pMR_PROVIDERS_SCIF_SCIF_H

#include <cstdint>

namespace pMR
{
    namespace scif
    {
        constexpr std::size_t Padding = 4096;
        constexpr std::size_t DMAThreshold = 32 * 1024;

        std::size_t padSize(std::size_t const size);
    }
}
#endif // pMR_PROVIDERS_SCIF_SCIF_H
