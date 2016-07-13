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

namespace pMR { namespace verbs
{
    enum Verbs
    {
        VerbsRecvWRID = 1,
        VerbsSendWRID = 2,
        VerbsRDMAWRID = 3,
        VerbsMaxSend = 1,
        VerbsMaxRecv = 1024,
        VerbsMaxCQEntry = 1024,
        VerbsInitialPostRecv = 512,
        VerbsMaxSendSG = 1,
        VerbsMaxRecvSG = 1,
        VerbsServiceLevel = 0,
        VerbsPSN = 0,
        VerbsTimeout = 14,
        VerbsRetry = 7,
        VerbsPKeyIndex = 0,
        VerbsRDAtomic = 1,
        VerbsRNRTimer = 12,
        VerbsSrcPath = 0,
        VerbsStaticRate = 0,
        VerbsSGIDIndex = 0,
        VerbsHopLimit = 1,
        VerbsGlobal = 1,
        VerbsMADBlockSize = 256,
        VerbsMADBlockGRHSize = 296,
        VerbsGRHSize = VerbsMADBlockGRHSize - VerbsMADBlockSize,
        VerbsDefaultQP1QKey = 0x80010000
    };
}}
#endif // pMR_PROVIDERS_VERBS_VERBS_H
