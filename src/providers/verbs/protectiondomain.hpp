#ifndef pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H
#define pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H

extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR { namespace verbs
{
    class ProtectionDomain
    {
        public:
            ProtectionDomain(Context&);
            ProtectionDomain(const ProtectionDomain&) = delete;
            ProtectionDomain(ProtectionDomain&&) = delete;
            ProtectionDomain& operator=(const ProtectionDomain&) = delete;
            ProtectionDomain& operator=(ProtectionDomain&&) = delete;
            ~ProtectionDomain() noexcept;
            ibv_pd* get();
            ibv_pd const* get() const;
        private:
            ibv_pd *mProtectionDomain = nullptr;
    };
}}
#endif // pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H
