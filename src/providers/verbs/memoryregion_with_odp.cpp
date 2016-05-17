#include "memoryregion.hpp"

pMR::verbs::ODP::ODP(Context& context)
{
    std::uint32_t const capMask = IBV_ODP_SUPPORT_SEND | IBV_ODP_SUPPORT_RECV;

    ibv_device_attr_ex extendedAttr;

    if(ibv_query_device_ex(context.get(), NULL, &extendedAttr))
    {
        throw pMR::verbs::DeviceExtendedQueryError();
    }

    if(!(extendedAttr.odp_caps.general_caps & IBV_ODP_SUPPORT) ||
        (extendedAttr.odp_caps.per_transport_caps.rc_odp_caps & capMask)
        != capMask)
    {
        mHasODP = false;
    }
    else
    {
        mHasODP = true;
    }
}

int pMR::verbs::updateMemoryRegionAccessODP(int access)
{
    return access | IBV_ACCESS_ON_DEMAND;
}

const char* pMR::verbs::DeviceExtendedQueryError::what() const throw()
{
    return "Unable to (extended) query device."
}
