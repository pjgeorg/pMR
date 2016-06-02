#ifndef pMR_PROVIDERS_VERBS_QUEUEPAIR_H
#define pMR_PROVIDERS_VERBS_QUEUEPAIR_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "protectiondomain.hpp"
#include "completionqueue.hpp"
#include "connectionaddress.hpp"

namespace pMR { namespace verbs
{
    class ConnectionAddress;
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
            ~QueuePair() noexcept;
            ibv_qp* get();
            ibv_qp const* get() const;
            std::uint32_t getQPN() const;
            void setStateINIT(std::uint8_t const portNumber);
            void setStateRTR(std::uint8_t const portNumber,
                    ConnectionAddress const&);
            void setStateRTS();
            std::uint32_t getMaxInlineDataSize() const;
        private:
            ibv_qp *mQueuePair = nullptr;
    };
}}
#endif // pMR_PROVIDERS_VERBS_QUEUEPAIR_H
