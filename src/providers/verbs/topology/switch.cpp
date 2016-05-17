#include "switch.hpp"
#include "mad/switch.hpp"

std::uint16_t pMR::verbs::getSwitchLID(Device const &device,
        std::uint8_t const portNumber)
{
    Context context(device);
    return getSwitchLID(context, portNumber);
}

std::uint16_t pMR::verbs::getSwitchLID(Context &context,
        std::uint8_t const portNumber)
{
    mad::SwitchLID switchLID(context, portNumber);
    return switchLID.getSwitchLID();
}
