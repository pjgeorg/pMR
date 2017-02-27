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

#ifndef pMR_PROVIDERS_PSM2_UUID_H
#define pMR_PROVIDERS_PSM2_UUID_H

#include <cstdint>
extern "C" {
#include <psm2.h>
}

namespace pMR
{
    namespace psm2
    {
        class UUID
        {
        public:
            UUID();
            ~UUID() = default;
            psm2_uuid_t &get()
            {
                return mUUID;
            }
            psm2_uuid_t const &get() const
            {
                return mUUID;
            }

        private:
            psm2_uuid_t mUUID;
        };
    }
}
#endif // pMR_PROVIDERS_PSM2_UUID_H
