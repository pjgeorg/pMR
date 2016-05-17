#ifndef pMR_PROVIDERS_VERBS_GID_H
#define pMR_PROVIDERS_VERBS_GID_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR { namespace verbs
{
    class GID
    {
        public:
            GID() = default;
            GID(Context&, std::uint8_t const portNumber);
            GID(ibv_gid const gID);
            void setGUID(std::uint64_t const);
            void setSubnetPrefix(std::uint64_t const);
            ibv_gid const& get() const;
            std::uint64_t getGUID() const;
            std::uint64_t getSubnetPrefix() const;
        private:
            ibv_gid mGID;
    };
}}
#endif // pMR_PROVIDERS_VERBS_GID_H
