#ifndef pMR_PROVIDERS_VERBS_CONNECTIONADDRESS_H
#define pMR_PROVIDERS_VERBS_CONNECTIONADDRESS_H

#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "context.hpp"
#include "queuepair.hpp"
#include "gid.hpp"

namespace pMR
{
    class Target;
}

namespace pMR { namespace verbs
{
    class QueuePair;
    class ConnectionAddress
    {
        public:
            ConnectionAddress() = default;
            ConnectionAddress(Context&, QueuePair const&,
                    std::uint8_t const portNumber);
            ConnectionAddress(ConnectionAddress const&, QueuePair const&);
            void setQPN(std::uint32_t const);
            void setLID(std::uint16_t const);
            void setGUID(std::uint64_t const);
            void setSubnetPrefix(std::uint64_t const);
            void setMTU(ibv_mtu const);
            std::uint32_t getQPN() const;
            std::uint16_t getLID() const;
            ibv_gid getGID() const;
            std::uint64_t getGUID() const;
            std::uint64_t getSubnetPrefix() const;
            ibv_mtu getMTU() const;
        private:
            std::uint32_t mQPN;
            GID mGID;
            std::uint16_t mLID;
            ibv_mtu mMTU;
    };

    void exchangeConnectionAddress(Target const &target,
            ConnectionAddress const &originActiveAddress,
            ConnectionAddress const &originPassiveAddress,
            ConnectionAddress &targetActiveAddress,
            ConnectionAddress &targetPassiveAddress);
}}
#endif // pMR_PROVIDERS_VERBS_CONNECTIONADDRESS_H
