#include "address.hpp"

pMR::verbs::mad::Address::Address(std::uint8_t const portNumber)
{
    mAddress.is_global = 0;
    mAddress.src_path_bits = 0;
    mAddress.port_num = portNumber;
}

ibv_ah_attr* pMR::verbs::mad::Address::get()
{
    return &mAddress;
}

ibv_ah_attr const* pMR::verbs::mad::Address::get() const
{
    return &mAddress;
}

pMR::verbs::mad::SubnetManager::SubnetManager(PortAttributes &portAttributes,
        std::uint8_t const portNumber)
    :   Address(portNumber)
{
    mAddress.dlid = portAttributes.getSubnetManagerLID();
    mAddress.sl = portAttributes.getSubnetManagerServiceLevel();
}

pMR::verbs::mad::AddressHandle::AddressHandle(
        ProtectionDomain &protectionDomain, Address &address)
{
    mAddressHandle = ibv_create_ah(protectionDomain.get(), address.get());
}

pMR::verbs::mad::AddressHandle::~AddressHandle()
{
    ibv_destroy_ah(mAddressHandle);
}

ibv_ah* pMR::verbs::mad::AddressHandle::get()
{
    return mAddressHandle;
}

ibv_ah const* pMR::verbs::mad::AddressHandle::get() const
{
    return mAddressHandle;
}
