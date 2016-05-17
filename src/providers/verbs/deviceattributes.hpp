#ifndef pMR_PROVIDERS_VERBS_DEVICEATTRIBUTES_H
#define pMR_PROVIDERS_VERBS_DEVICEATTRIBUTES_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR { namespace verbs
{
    class DeviceAttributes
    {
        public:
            DeviceAttributes(Context&);
            ~DeviceAttributes() = default;
            std::uint64_t getNodeGUID() const;
            std::uint8_t getPortCount() const;
        private:
            ibv_device_attr mDeviceAttributes;
    };
}}
#endif // pMR_PROVIDERS_VERBS_DEVICEATTRIBUTES_H
