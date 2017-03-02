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
        constexpr std::uint64_t RecvWRID = 1;
        constexpr std::uint64_t SendWRID = 2;
        constexpr std::uint64_t WriteWRID = 3;
        constexpr int MaxCQEntry = 16;
        constexpr std::uint32_t MaxSend = 1;
        constexpr std::uint32_t MaxRecv = 32;
        constexpr std::uint32_t MaxSendSG = 1;
        constexpr std::uint32_t MaxRecvSG = 1;
        constexpr std::uint32_t MaxInlineDataSize = VERBS_INLINE;
        constexpr std::uint16_t PKeyIndex = 0;
        constexpr ibv_mtu MTU = VERBS_MTU;
        constexpr std::uint32_t PSN = 0;
        constexpr std::uint8_t DestRDAtomic = 1;
        constexpr std::uint8_t RNRTimer = 12;
        constexpr std::uint8_t ServiceLevel = 0;
        constexpr std::uint8_t SrcPath = 0;
        constexpr std::uint8_t StaticRate = 0;
        constexpr std::uint8_t Global = 1;
        constexpr std::uint8_t SGIDIndex = 0;
        constexpr std::uint8_t HopLimit = 1;
        constexpr std::uint8_t RDAtomic = 1;
        constexpr std::uint8_t Timeout = 14;
        constexpr std::uint8_t RetryCounter = 7;
        constexpr std::uint8_t RNRRetry = 7;
        constexpr int InitialPostRecv = 16;
        constexpr std::uint32_t DefaultQP1QKey = 0x80010000;
        constexpr int MADPollCQRetry = 1000;
        constexpr int MADBlockSize = 256;
        constexpr int MADBlockGRHSize = 296;
        constexpr int GRHSize = MADBlockGRHSize - MADBlockSize;
    }
}
#endif // pMR_PROVIDERS_VERBS_VERBS_H
