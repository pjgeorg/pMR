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

#include <limits>
#include <random>
#include "singleton.hpp"

namespace pMR
{
    class Seed
    {
    protected:
        ~Seed() = default;
        static std::random_device mSeed;
        static std::default_random_engine mEngine;
    };

    template<typename T, bool = std::is_integral<T>::value>
    class Random;

    template<typename T>
    class Random<T, true> : Seed
    {
    public:
        T getRandomNumber();

    private:
        static std::uniform_int_distribution<T> mDistribution;
    };

    template<typename T>
    class Random<T, false> : Seed
    {
    public:
        T getRandomNumber();

    private:
        static std::uniform_real_distribution<T> mDistribution;
    };

    template<typename T>
    T getRandomNumber()
    {
        return Random<T>().getRandomNumber();
    }
}

template<typename T>
T pMR::Random<T, true>::getRandomNumber()
{
    return mDistribution(mEngine);
}

template<typename T>
std::uniform_int_distribution<T> pMR::Random<T, true>::mDistribution(
    std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

template<typename T>
T pMR::Random<T, false>::getRandomNumber()
{
    return mDistribution(mEngine);
}

template<typename T>
std::uniform_real_distribution<T> pMR::Random<T, false>::mDistribution(
    std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
#endif // pMR_MISC_RANDOM_H
