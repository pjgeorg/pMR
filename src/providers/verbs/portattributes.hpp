#ifndef pMR_PROVIDERS_VERBS_PORTATTRIBUTES_H
#define pMR_PROVIDERS_VERBS_PORTATTRIBUTES_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR { namespace verbs
{
    class PortAttributes
    {
        public:
            PortAttributes(Context&, std::uint8_t const portNumber);
            ~PortAttributes() = default;
            std::uint16_t getLID() const;
            ibv_mtu getMTU() const;
            std::uint16_t getSubnetManagerLID() const;
            std::uint8_t getSubnetManagerServiceLevel() const;
        private:
            ibv_port_attr mPortAttributes;
    };
}}
#endif // pMR_PROVIDERS_VERBS_PORTATTRIBUTES_H
