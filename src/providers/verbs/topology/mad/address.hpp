#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_ADDRESS_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_ADDRESS_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "../../portattributes.hpp"
#include "../../protectiondomain.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class Address
    {
        public:
            Address(std::uint8_t const portNumber);
            ~Address() = default;
            ibv_ah_attr* get();
            ibv_ah_attr const* get() const;
        protected:
            ibv_ah_attr mAddress = {};
    };

    class SubnetManager : public Address
    {
        public:
            SubnetManager(PortAttributes&, std::uint8_t const portNumber);
            ~SubnetManager() = default;
    };

    class AddressHandle
    {
        public:
            AddressHandle(ProtectionDomain&, Address&);
            AddressHandle(const AddressHandle&) = delete;
            AddressHandle(AddressHandle&&) = delete;
            AddressHandle& operator=(const AddressHandle&) = delete;
            AddressHandle& operator=(AddressHandle&&) = delete;
            ~AddressHandle();
            ibv_ah* get();
            ibv_ah const* get() const;
        private:
            ibv_ah *mAddressHandle = nullptr;
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_ADDRESS_H
