#ifndef pMR_PROVIDERS_VERBS_MEMORYREGION_H
#define pMR_PROVIDERS_VERBS_MEMORYREGION_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "protectiondomain.hpp"

namespace pMR { namespace verbs
{
    class ODP
    {
        public:
            ODP(Context&);
            ~ODP() = default;
            bool isTrue() const;
        private:
            bool mHasODP;
    };

    int updateMemoryRegionAccessODP(int access);

    class MemoryRegion
    {
        public:
            MemoryRegion(Context &context, ProtectionDomain &protectionDomain,
                    void* buffer, std::uint32_t const size, int access);
            MemoryRegion(const MemoryRegion&) = delete;
            MemoryRegion(MemoryRegion&&) = delete;
            MemoryRegion& operator=(const MemoryRegion&) = delete;
            MemoryRegion& operator=(MemoryRegion&&) = delete;
            ~MemoryRegion() noexcept;
            ibv_mr* get();
            ibv_mr const* get() const;
            std::uint64_t getAddress() const;
            std::uint32_t getLKey() const;
            std::uint32_t getRKey() const;
            std::uint32_t getLength() const;
        private:
            ibv_mr *mMemoryRegion = nullptr;
            void registerMemoryRegion(ProtectionDomain&, void* buffer,
                    std::uint32_t const size, int const access);
    };
}}
#endif // pMR_PROVIDERS_VERBS_MEMORYREGION_H
