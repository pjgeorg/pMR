#include "gid.hpp"
#include <stdexcept>
#include "verbs.hpp"

pMR::verbs::GID::GID(Context &context, std::uint8_t const portNumber)
{
    if(ibv_query_gid(context.get(), portNumber, VerbsSGIDIndex, &mGID))
    {
        throw std::runtime_error("pMR: Unable to query GID.");
    }
}

pMR::verbs::GID::GID(ibv_gid const gID)
    :   mGID(gID) { }

void pMR::verbs::GID::setGUID(std::uint64_t const GUID)
{
    mGID.global.interface_id = GUID;
}

void pMR::verbs::GID::setSubnetPrefix(std::uint64_t const subnetPrefix)
{
    mGID.global.subnet_prefix = subnetPrefix;
}

ibv_gid const& pMR::verbs::GID::get() const
{
    return mGID;
}

std::uint64_t pMR::verbs::GID::getGUID() const
{
    return mGID.global.interface_id;
}

std::uint64_t pMR::verbs::GID::getSubnetPrefix() const
{
    return mGID.global.subnet_prefix;
}
