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

#ifndef pMR_MISC_RANDOM_H
#define pMR_MISC_RANDOM_H

#include <random>
#include <limits>
#include "singleton.hpp"

namespace pMR
{
    template<typename T>
    class Random
    {
        public:
            Random()
                :   mEngine(mSeed()),
                    mDistribution(std::numeric_limits<T>::min(),
                            std::numeric_limits<T>::max()) { }
            ~Random() = default;
            T getRandomNumber()
            {
                return mDistribution(mEngine);
            }
        private:
            std::random_device mSeed;
            std::default_random_engine mEngine;
            std::uniform_int_distribution<T> mDistribution;
    };

    template<typename T>
    T getRandomNumber()
    {
        return Singleton<Random<T>>::Instance().getRandomNumber();
    }
}
#endif // pMR_MISC_RANDOM_H
