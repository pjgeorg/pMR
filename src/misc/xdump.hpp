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

#ifndef pMR_MISC_XDUMP_H
#define pMR_MISC_XDUMP_H

#include <cstdint>
#include <vector>
#include <string>

namespace pMR
{
    std::string xdump(void const *data, std::uint64_t sizeByte);

    template<typename T>
    std::string xdump(std::vector<T> const &vector)
    {
        return xdump(vector.data(), vector.size() * sizeof(T));
    }
}
#endif // pMR_MISC_XDUMP_H
