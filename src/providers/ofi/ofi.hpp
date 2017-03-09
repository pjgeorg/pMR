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

#ifndef pMR_PROVIDERS_OFI_OFI_H
#define pMR_PROVIDERS_OFI_OFI_H

#include <cstdint>

namespace pMR
{
    namespace OFI
    {
        constexpr int cVersionMajor = 1;
        constexpr int cVersionMinor = 3;
        constexpr std::size_t cEventQueueSize = 2;
        constexpr std::size_t cAVCount = 16;
        constexpr std::size_t cReserveSizeCompletion = 128;
    }
}
#endif // pMR_PROVIDERS_OFI_OFI_H
