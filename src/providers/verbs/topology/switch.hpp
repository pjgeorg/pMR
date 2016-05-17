#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_SWITCH_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_SWITCH_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "../device.hpp"
#include "../context.hpp"

namespace pMR { namespace verbs
{
    std::uint16_t getSwitchLID(Device const&, std::uint8_t const portNumber);
    std::uint16_t getSwitchLID(Context&, std::uint8_t const portNumber);
}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_SWITCH_H
