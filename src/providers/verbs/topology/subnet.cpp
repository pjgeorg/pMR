#include "../topology.hpp"

std::uint16_t pMR::verbs::getSubnetPrefix(Device const &device,
        std::uint8_t const portNumber)
{
    Context context(device);
    return getSubnetPrefix(context, portNumber);
}

std::uint16_t pMR::verbs::getSubnetPrefix(Context &context,
        std::uint8_t const portNumber)
{
    GID const gID(context, portNumber);
    return getSubnetPrefix(gID);
}

std::uint16_t pMR::verbs::getSubnetPrefix(GID const &gID)
{
    return gID.getSubnetPrefix();
}
