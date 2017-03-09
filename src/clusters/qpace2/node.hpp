//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef pMR_CLUSTERS_QPACE2_NODE_H
#define pMR_CLUSTERS_QPACE2_NODE_H

#include <array>
#include <cstdint>
#include "../../providers/verbs/topology.hpp"

namespace pMR
{
    class Node
    {
    public:
        Node(Verbs::Device const &);
        Node(std::array<std::uint64_t, 4> const &);
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
