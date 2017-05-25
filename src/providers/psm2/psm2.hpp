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

#ifndef pMR_PROVIDERS_PSM2_PSM2_H
#define pMR_PROVIDERS_PSM2_PSM2_H

#include <array>
#include <cstdint>
extern "C" {
#include <psm2_mq.h>
}

namespace pMR
{
    namespace PSM2
    {
        constexpr std::int64_t cEndpointCloseTimeout = 5 * 1000000000LL;
        constexpr char cUUIDEnv[] = "PSM2_UUID";
        constexpr char cDefaultUUID[] = "00FF00FF-0000-0000-0000-00FF00FF00FF";
        constexpr std::uint32_t cEagerThreshold = PSM2_EAGER_THRESHOLD;
        constexpr char cLocalRankIDEnv[] = "MPI_LOCALRANKID";
        constexpr char cLocalNRanksEnv[] = "MPI_LOCALNRANKS";

        // We use the additional 32bit to distinguish from any other library
        // E.g. OFI is using these for (L->R):
        // 0-7: Msg Type (RMA, MSG, etc.)
        // 8-31: SEQ
        // 32-47: SRC pEP
        // 48-63: DST pEP
        constexpr std::uint32_t cExtraTag = 0xFFFFFFFF;
        constexpr psm2_mq_tag_t cTagSelector = {
            0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    }
}
#endif // pMR_PROVIDERS_PSM2_PSM2_H
