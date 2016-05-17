#include "memoryaddress.hpp"

void pMR::verbs::MemoryAddress::set(MemoryRegion const &memoryRegion)
{
    std::get<0>(mData) =
        reinterpret_cast<std::uint64_t>(memoryRegion.get()->addr);
    std::get<1>(mData) = memoryRegion.get()->rkey;
    std::get<2>(mData) = memoryRegion.get()->length;
}

std::uint64_t pMR::verbs::MemoryAddress::getAddress() const
{
    return std::get<0>(mData);
}

std::uint32_t pMR::verbs::MemoryAddress::getRKey() const
{
    return static_cast<std::uint32_t>(std::get<1>(mData));
}

std::uint32_t pMR::verbs::MemoryAddress::getLength() const
{
    return static_cast<std::uint32_t>(std::get<2>(mData));
}

std::uint64_t* pMR::verbs::MemoryAddress::rawData()
{
    return mData.data();
}

std::uint64_t const* pMR::verbs::MemoryAddress::rawData() const
{
    return mData.data();
}

std::size_t pMR::verbs::MemoryAddress::size() const
{
    return sizeof(mData);
}
