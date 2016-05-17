#include "../topology.hpp"

std::uint16_t pMR::verbs::getLID(Device const &device,
        std::uint8_t const portNumber)
{
    Context context(device);
    return getLID(context, portNumber);
}

std::uint16_t pMR::verbs::getLID(Context &context,
        std::uint8_t const portNumber)
{
    PortAttributes const portAttributes(context, portNumber);
    return getLID(portAttributes);
}

std::uint16_t pMR::verbs::getLID(PortAttributes const &portAttributes)
{
    return portAttributes.getLID();
}
