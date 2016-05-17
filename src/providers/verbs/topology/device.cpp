#include "../topology.hpp"

std::uint64_t pMR::verbs::getNodeGUID(Device const &device)
{
    Context context(device);
    return getNodeGUID(context);
}

std::uint64_t pMR::verbs::getNodeGUID(Context &context)
{
    DeviceAttributes const deviceAttributes(context);
    return getNodeGUID(deviceAttributes);
}

std::uint64_t pMR::verbs::getNodeGUID(DeviceAttributes const &deviceAttributes)
{
    return deviceAttributes.getNodeGUID();
}

std::uint8_t pMR::verbs::getPortCount(Device const &device)
{
    Context context(device);
    return getPortCount(context);
}

std::uint8_t pMR::verbs::getPortCount(Context &context)
{
    DeviceAttributes const deviceAttributes(context);
    return getPortCount(deviceAttributes);
}

std::uint8_t pMR::verbs::getPortCount(DeviceAttributes const &deviceAttributes)
{
    return deviceAttributes.getPortCount();
}
