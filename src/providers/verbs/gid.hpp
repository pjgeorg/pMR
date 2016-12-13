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

#ifndef pMR_PROVIDERS_VERBS_GID_H
#define pMR_PROVIDERS_VERBS_GID_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include "context.hpp"

namespace pMR
{
    namespace verbs
    {
        class GID
        {
        public:
            GID() = default;
            GID(Context &, std::uint8_t const portNumber);
            GID(ibv_gid const gID);
            void setGUID(std::uint64_t const);
            void setSubnetPrefix(std::uint64_t const);
            ibv_gid const &get() const;
            std::uint64_t getGUID() const;
            std::uint64_t getSubnetPrefix() const;

        private:
            ibv_gid mGID;
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_GID_H
