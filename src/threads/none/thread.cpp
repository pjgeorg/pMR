#include "../thread.hpp"

pMR::thread::ScopedLock::ScopedLock() { }

pMR::thread::ScopedLock::~ScopedLock() { }

bool pMR::thread::isThreaded()
{
    return false;
}

bool pMR::thread::isSerialized()
{
    return false;
}
