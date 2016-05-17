#include "time.hpp"
#include <stdexcept>

double pMR::getTimeInSeconds()
{
    timespec timeSpec;
    if(clock_gettime(CLOCK_MONOTONIC, &timeSpec))
    {
        throw std::runtime_error("Unable to get current time.");
    }
    return static_cast<double>(timeSpec.tv_nsec)/1e9 + timeSpec.tv_sec;
}
