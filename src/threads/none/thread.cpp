#include "../thread.hpp"

pMR::thread::ScopedLock::ScopedLock() { }

pMR::thread::ScopedLock::~ScopedLock() { }

bool pMR::thread::isMT()
{
    return false;
}
