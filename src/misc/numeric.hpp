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
