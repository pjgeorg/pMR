#ifndef pMR_CLUSTERS_QPACE2_NODE_H
#define pMR_CLUSTERS_QPACE2_NODE_H

#include <array>
#include <cstdint>

namespace pMR
{
    namespace verbs
    {
        class Device;
    }

    class Node
    {
        public:
            Node(verbs::Device const&);
            Node(std::array<std::uint64_t, 4> const&);
            ~Node() = default;
            std::array<std::uint64_t, 4> flatten() const;
            std::uint64_t getNodeGUID() const;
            std::uint16_t getSwitchLID(std::uint8_t const portNumber) const;
            std::uint16_t getSCIFNodeID() const;
        private:
            std::uint64_t mNodeGUID;
            std::array<std::uint16_t, 2> mSwitchLID;
            std::uint16_t mSCIFNodeID;
    };
}
#endif // pMR_CLUSTERS_QPACE2_NODE_H
