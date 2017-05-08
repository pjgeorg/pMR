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

#ifndef pMR_MISC_TYPE_H
#define pMR_MISC_TYPE_H

#include <iterator>

namespace pMR
{
    template<class Iterator>
    constexpr bool isRandomAccessIterator();
}

template<class Iterator>
constexpr bool pMR::isRandomAccessIterator()
{
    return std::is_base_of<std::random_access_iterator_tag,
        typename std::iterator_traits<Iterator>::iterator_category>::value;
}
#endif // pMR_MISC_TYPE_H
