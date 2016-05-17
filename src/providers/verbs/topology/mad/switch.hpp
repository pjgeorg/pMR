#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_SWITCH_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_SWITCH_H

#include "mad.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class SwitchLID : public MAD
    {
        public:
            SwitchLID(Context&, std::uint8_t const portNumber);
            SwitchLID(const SwitchLID&) = delete;
            SwitchLID(SwitchLID&&) = delete;
            SwitchLID& operator=(const SwitchLID&) = delete;
            SwitchLID& operator=(SwitchLID&&) = delete;
            ~SwitchLID() = default;
            bool validate();
            int getSwitchLID();
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_SWITCH_H
