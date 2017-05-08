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

#ifndef pMR_PROVIDERS_VERBS_VERBS_H
#define pMR_PROVIDERS_VERBS_VERBS_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}

namespace pMR
{
    namespace Verbs
    {
        constexpr std::uint64_t cRecvWRID = 1;
        constexpr std::uint64_t cSendWRID = 2;
        constexpr std::uint64_t cWriteWRID = 3;
        constexpr int cMaxCQEntry = 16;
        constexpr std::uint32_t cMaxSend = 1;
        constexpr std::uint32_t cMaxRecv = 32;
        constexpr std::uint32_t cMaxSendSG = 1;
        constexpr std::uint32_t cMaxRecvSG = 1;
        constexpr std::uint32_t cMaxInlineDataSize = VERBS_INLINE;
        constexpr std::uint16_t cPKeyIndex = 0;
        constexpr ibv_mtu cMTU = VERBS_MTU;
        constexpr std::uint32_t cPSN = 0;
        constexpr std::uint8_t cDestRDAtomic = 1;
        constexpr std::uint8_t cRNRTimer = 12;
        constexpr std::uint8_t cServiceLevel = 0;
        constexpr std::uint8_t cSrcPath = 0;
        constexpr std::uint8_t cStaticRate = 0;
        constexpr std::uint8_t cGlobal = 1;
        constexpr std::uint8_t cSGIDIndex = 0;
        constexpr std::uint8_t cHopLimit = 1;
        constexpr std::uint8_t cRDAtomic = 1;
        constexpr std::uint8_t cTimeout = 14;
        constexpr std::uint8_t cRetryCounter = 7;
        constexpr std::uint8_t cRNRRetry = 7;
        constexpr int cInitialPostRecv = 16;
        constexpr std::uint32_t cDefaultQP1QKey = 0x80010000;
        constexpr int cMADPollCQRetry = 1000;
        constexpr int cMADBlockSize = 256;
        constexpr int cMADBlockGRHSize = 296;
        constexpr int cGRHSize = cMADBlockGRHSize - cMADBlockSize;
    }
}
#endif // pMR_PROVIDERS_VERBS_VERBS_H
