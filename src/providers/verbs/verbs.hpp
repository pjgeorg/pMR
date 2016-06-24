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
