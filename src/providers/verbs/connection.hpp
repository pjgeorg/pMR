#ifndef pMR_PROVIDERS_VERBS_CONNECTION_H
#define pMR_PROVIDERS_VERBS_CONNECTION_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"
#include "protectiondomain.hpp"
#include "completionqueue.hpp"
#include "queuepair.hpp"
#include "connectionaddress.hpp"
#include "memoryaddress.hpp"
#include "memoryregion.hpp"

namespace pMR { namespace verbs
{
    class Connection
    {
        public:
            Connection(Target const &target,
                    Device const &device, std::uint8_t const portNumber = 1);
            Connection(const Connection&) = delete;
            Connection(Connection&&) = delete;
            Connection& operator=(const Connection&) = delete;
            Connection& operator=(Connection&&) = delete;
            ~Connection() = default;
            Context& getContext();
            Context const& getContext() const;
            ProtectionDomain& getProtectionDomain();
            ProtectionDomain const& getProtectionDomain() const;
            void setLocalMemoryAddress(MemoryRegion const&);
            MemoryAddress const& getRemoteMemoryAddress() const;

            void postRecvSyncRequestToPassive();
            void postSendAddrRequestToPassive();

            void postRecvAddrRequestToActive();
            void postSendSyncRequestToActive();
            void postRDMAWriteRequestToActive(MemoryRegion const &memoryRegion,
                    MemoryAddress const &remoteMemoryAddress);

            void pollActiveCompletionQueue();
            void pollPassiveCompletionQueue();

            void initFence();
            void waitFence();
        private:
            Context mContext;
            ProtectionDomain mProtectionDomain;
            CompletionQueue mActiveCompletionQueue;
            CompletionQueue mPassiveCompletionQueue;
            QueuePair mActiveQueuePair;
            QueuePair mPassiveQueuePair;
            MemoryAddress mLocalMemoryAddress;
            MemoryAddress mRemoteMemoryAddress;
            MemoryRegion mSendMemoryRegion;
            MemoryRegion mRecvMemoryRegion;
            std::uint32_t mMaxInlineDataSize = 0;
            void postRecvRequest(QueuePair &queuePair,
                    ibv_sge *scatterGatherList, int const numEntries);
            void postSendRequest(QueuePair &queuePair,
                    ibv_sge *scatterGatherList, int const numEntries);
    };

    class ScatterGatherList
    {
        public:
            ScatterGatherList(MemoryRegion const&);
            ibv_sge* get();
            ibv_sge const* get() const;
            std::uint32_t getLength() const;
            int getNumEntries() const;
        private:
            ibv_sge mScatterGatherList;
    };
}}
#endif // pMR_PROVIDERS_VERBS_CONNECTION_H
