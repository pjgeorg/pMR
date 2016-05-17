#ifndef pMR_MISC_PROFILING
#define pMR_MISC_PROFILING

#ifdef PROFILING
    #include "time.hpp"
    #define PROF_START(x) x -= pMR::getTimeInSeconds()
    #define PROF_STOP(x) x += pMR::getTimeInSeconds()
    #define PROF_START_THREAD(x) (threadID) ? : x -= pMR::getTimeInSeconds()
    #define PROF_STOP_THREAD(x) (threadID) ? : x += pMR::getTimeInSeconds()
    #define PROF_COUNT(x) ++x
#else
    #define PROF_START(x)
    #define PROF_STOP(x)
    #define PROF_START_THREAD(x)
    #define PROF_STOP_THREAD(x)
    #define PROF_COUNT(x)
#endif // PROFILING

#endif // pMR_MISC_PROFILING
