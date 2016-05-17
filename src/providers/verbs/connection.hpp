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

            void postRecvRequest();
            void postSendRequest();
            void postRDMAWriteRequest(MemoryRegion const &memoryRegion,
                    MemoryAddress const &remoteMemoryAddress);
            void pollSendCompletionQueue();
            void pollRecvCompletionQueue();

            void initFence();
            void waitFence();
        private:
            Context mContext;
            ProtectionDomain mProtectionDomain;
            CompletionQueue mSendCompletionQueue;
            CompletionQueue mRecvCompletionQueue;
            QueuePair mQueuePair;
            MemoryAddress mLocalMemoryAddress;
            MemoryAddress mRemoteMemoryAddress;
            MemoryRegion mSendMemoryRegion;
            MemoryRegion mRecvMemoryRegion;
            void postRecvRequest(QueuePair &queuePair,
                    ibv_sge *scatterGatherList, int const length);
            void postSendRequest(QueuePair &queuePair,
                    ibv_sge *scatterGatherList, int const length);
    };

    class ScatterGatherList
    {
        public:
            ScatterGatherList(MemoryRegion const&);
            ibv_sge* get();
            ibv_sge const* get() const;
        private:
            ibv_sge mScatterGatherList;
    };
}}
#endif // pMR_PROVIDERS_VERBS_CONNECTION_H
