#include "portattributes.hpp"
#include <stdexcept>

pMR::verbs::PortAttributes::PortAttributes(Context &context,
        std::uint8_t const portNumber)
{
    if(ibv_query_port(context.get(), portNumber, &mPortAttributes))
    {
        throw std::runtime_error("pMR: Unable to query IB port.");
    }
}

std::uint16_t pMR::verbs::PortAttributes::getLID() const
{
    return mPortAttributes.lid;
}

ibv_mtu pMR::verbs::PortAttributes::getMTU() const
{
    return mPortAttributes.active_mtu;
}

std::uint16_t pMR::verbs::PortAttributes::getSubnetManagerLID() const
{
    return mPortAttributes.sm_lid;
}

std::uint8_t pMR::verbs::PortAttributes::getSubnetManagerServiceLevel() const
{
    return mPortAttributes.sm_sl;
}
