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

#ifndef pMR_PROVIDERS_VERBS_ODP_H
#define pMR_PROVIDERS_VERBS_ODP_H

#ifdef VERBS_ODP
namespace pMR
{
    namespace verbs
    {
        class ODP
        {
        public:
            ODP(Context &);
            ~ODP() = default;
            bool isTrue() const;

        private:
            bool mHasODP = false;
        };

        int updateMemoryRegionAccessODP(Context &context, int const access);
    }
}
#endif // VERBS_ODP

#endif // pMR_PROVIDERS_VERBS_ODP_H
