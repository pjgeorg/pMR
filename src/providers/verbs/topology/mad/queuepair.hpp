#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_QUEUEPAIR_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_QUEUEPAIR_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "../../protectiondomain.hpp"
#include "../../completionqueue.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class QueuePair
    {
        public:
            QueuePair(ProtectionDomain&,
                    CompletionQueue &sendCompletionQueue,
                    CompletionQueue &recvCompletionQueue);
            QueuePair(const QueuePair&) = delete;
            QueuePair(QueuePair&&) = delete;
            QueuePair& operator=(const QueuePair&) = delete;
            QueuePair& operator=(QueuePair&&) = delete;
            ~QueuePair();
            ibv_qp* get();
            ibv_qp const* get() const;
            void setStateINIT(std::uint8_t const portNumber);
            void setStateRTR();
            void setStateRTS();
        private:
            ibv_qp *mQueuePair = nullptr;
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_QUEUEPAIR_H
