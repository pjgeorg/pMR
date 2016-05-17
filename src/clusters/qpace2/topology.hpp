#ifndef pMR_CLUSTERS_QPACE2_TOPOLOGY_H
#define pMR_CLUSTERS_QPACE2_TOPOLOGY_H

#include <cstdint>

namespace pMR
{
    class Node;
    std::uint8_t detectBestPort(Node const &origin, Node const &target);
}
#endif // pMR_CLUSTERS_QPACE2_TOPOLOGY_H
