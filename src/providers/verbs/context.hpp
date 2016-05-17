#ifndef pMR_PROVIDERS_VERBS_CONTEXT_H
#define pMR_PROVIDERS_VERBS_CONTEXT_H

extern "C"
{
#include <infiniband/verbs.h>
}
#include "device.hpp"

namespace pMR { namespace verbs
{
    class Context
    {
        public:
            Context(Device const&);
            Context(const Context&) = delete;
            Context(Context&&) = delete;
            Context& operator=(const Context&) = delete;
            Context& operator=(Context&&) = delete;
            ~Context() noexcept;
            ibv_context* get();
            ibv_context const* get() const;
        private:
            ibv_context *mContext = nullptr;
    };
}}
#endif // pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H
