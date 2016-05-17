#include "deviceattributes.hpp"
#include <stdexcept>

pMR::verbs::DeviceAttributes::DeviceAttributes(Context &context)
{
    if(ibv_query_device(context.get(), &mDeviceAttributes))
    {
        throw std::runtime_error("pMR: Unable to query device.");
    }
}

std::uint64_t pMR::verbs::DeviceAttributes::getNodeGUID() const
{
    return mDeviceAttributes.node_guid;
}

std::uint8_t pMR::verbs::DeviceAttributes::getPortCount() const
{
    return mDeviceAttributes.phys_port_cnt;
}
