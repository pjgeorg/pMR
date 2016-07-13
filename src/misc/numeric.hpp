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

#ifndef pMR_MISC_NUMERIC_H
#define pMR_MISC_NUMERIC_H

namespace pMR
{
    template<typename T>
    constexpr T getGreatestCommonDivisor(T const m, T const n)
    {
        return (m < n) ?
            getGreatestCommonDivisor(n, m) : ((m % n == 0) ?
                    n : getGreatestCommonDivisor(n, m % n));
    }
    
    template<typename T>
    constexpr T getLeastCommonMultiple(T const m, T const n)
    {
        return getGreatestCommonDivisor(m, n) ?
            m / getGreatestCommonDivisor(m, n) * n : 0;
    }
}
#endif // pMR_MISC_NUMERIC_H
