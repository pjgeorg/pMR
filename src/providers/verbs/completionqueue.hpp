#ifndef pMR_PROVIDERS_VERBS_COMPLETIONQUEUE_H
#define pMR_PROVIDERS_VERBS_COMPLETIONQUEUE_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR { namespace verbs
{
    class CompletionQueue
    {
        public:
            CompletionQueue(Context&, int const size);
            CompletionQueue(const CompletionQueue&) = delete;
            CompletionQueue(CompletionQueue&&) = delete;
            CompletionQueue& operator=(const CompletionQueue&) = delete;
            CompletionQueue& operator=(CompletionQueue&&) = delete;
            ~CompletionQueue() noexcept;
            ibv_cq* get();
            ibv_cq const* get() const;
            void poll();
            bool poll(int count);
        private:
            ibv_cq *mCompletionQueue = nullptr;
    };
}}
#endif // pMR_PROVIDERS_VERBS_COMPLETIONQUEUE_H
