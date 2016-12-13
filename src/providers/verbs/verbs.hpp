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
    namespace verbs
    {
        constexpr std::uint64_t VerbsRecvWRID = 1;
        constexpr std::uint64_t VerbsSendWRID = 2;
        constexpr std::uint64_t VerbsWriteWRID = 3;
        constexpr int VerbsMaxCQEntry = 16;
        constexpr std::uint32_t VerbsMaxSend = 1;
        constexpr std::uint32_t VerbsMaxRecv = 32;
        constexpr std::uint32_t VerbsMaxSendSG = 1;
        constexpr std::uint32_t VerbsMaxRecvSG = 1;
        constexpr std::uint32_t VerbsMaxInlineDataSize = VERBS_INLINE;
        constexpr std::uint16_t VerbsPKeyIndex = 0;
        constexpr ibv_mtu VerbsMTU = VERBS_MTU;
        constexpr std::uint32_t VerbsPSN = 0;
        constexpr std::uint8_t VerbsDestRDAtomic = 1;
        constexpr std::uint8_t VerbsRNRTimer = 12;
        constexpr std::uint8_t VerbsServiceLevel = 0;
        constexpr std::uint8_t VerbsSrcPath = 0;
        constexpr std::uint8_t VerbsStaticRate = 0;
        constexpr std::uint8_t VerbsGlobal = 1;
        constexpr std::uint8_t VerbsSGIDIndex = 0;
        constexpr std::uint8_t VerbsHopLimit = 1;
        constexpr std::uint8_t VerbsRDAtomic = 1;
        constexpr std::uint8_t VerbsTimeout = 14;
        constexpr std::uint8_t VerbsRetryCounter = 7;
        constexpr std::uint8_t VerbsRNRRetry = 7;
        constexpr int VerbsInitialPostRecv = 16;
        constexpr std::uint32_t VerbsDefaultQP1QKey = 0x80010000;
        constexpr int VerbsMADPollCQRetry = 1000;
        constexpr int VerbsMADBlockSize = 256;
        constexpr int VerbsMADBlockGRHSize = 296;
        constexpr int VerbsGRHSize = VerbsMADBlockGRHSize - VerbsMADBlockSize;
    }
}
#endif // pMR_PROVIDERS_VERBS_VERBS_H
