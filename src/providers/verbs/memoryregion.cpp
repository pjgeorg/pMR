#include "memoryregion.hpp"
#include <stdexcept>
#include "../../misc/singleton.hpp"

bool pMR::verbs::ODP::isTrue() const
{
    return mHasODP;
}

pMR::verbs::MemoryRegion::MemoryRegion(Context &context,
        ProtectionDomain &protectionDomain,
        void* buffer, std::uint32_t size, int access)
{
    if(Singleton<ODP>::Instance(context).isTrue())
    {
        access = updateMemoryRegionAccessODP(access);
    }
    registerMemoryRegion(protectionDomain, buffer, size, access);
}

pMR::verbs::MemoryRegion::~MemoryRegion()
{
    ibv_dereg_mr(mMemoryRegion);
}

ibv_mr* pMR::verbs::MemoryRegion::get()
{
    return mMemoryRegion;
}

ibv_mr const* pMR::verbs::MemoryRegion::get() const
{
    return mMemoryRegion;
}

std::uint64_t pMR::verbs::MemoryRegion::getAddress() const
{
    return reinterpret_cast<std::uint64_t>(mMemoryRegion->addr);
}

std::uint32_t pMR::verbs::MemoryRegion::getLKey() const
{
    return {mMemoryRegion->lkey};
}

std::uint32_t pMR::verbs::MemoryRegion::getRKey() const
{
    return {mMemoryRegion->rkey};
}

std::uint32_t pMR::verbs::MemoryRegion::getLength() const
{
    return {static_cast<std::uint32_t>(mMemoryRegion->length)};
}

void pMR::verbs::MemoryRegion::registerMemoryRegion(
        ProtectionDomain &protectionDomain, void* buffer,
        std::uint32_t const size, int const access)
{
    mMemoryRegion = ibv_reg_mr(protectionDomain.get(), buffer, size, access);

    if(!mMemoryRegion)
    {
        throw std::runtime_error("pMR: Could not register Memory Region.");
    }
}
