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
