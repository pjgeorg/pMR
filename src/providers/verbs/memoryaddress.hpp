#ifndef pMR_PROVIDERS_VERBS_MEMORYADDRESS_H
#define pMR_PROVIDERS_VERBS_MEMORYADDRESS_H

#include <array>
#include <cstdint>
#include "memoryregion.hpp"

namespace pMR { namespace verbs
{
    class MemoryAddress
    {
        public:
            MemoryAddress() = default;
            void set(MemoryRegion const&);
            std::uint64_t getAddress() const;
            std::uint32_t getRKey() const;
            std::uint32_t getLength() const;
            std::uint64_t* rawData();
            std::uint64_t const* rawData() const;
            std::size_t size() const;
            ~MemoryAddress() = default;
        private:
            std::array<std::uint64_t,3> mData;
    };
}}
#endif // pMR_PROVIDERS_VERBS_MEMORYADDRESS_H
