#include "protectiondomain.hpp"
#include <stdexcept>

pMR::verbs::ProtectionDomain::ProtectionDomain(Context &context)
{
    mProtectionDomain = ibv_alloc_pd(context.get());

    if(!mProtectionDomain)
    {
        throw std::runtime_error("pMR: Could not allocate Protection Domain.");
    }
}

pMR::verbs::ProtectionDomain::~ProtectionDomain()
{
    ibv_dealloc_pd(mProtectionDomain);
}

ibv_pd* pMR::verbs::ProtectionDomain::get()
{
    return mProtectionDomain;
}

ibv_pd const* pMR::verbs::ProtectionDomain::get() const
{
    return mProtectionDomain;
}
