#include "../thread.hpp"

pMR::thread::ScopedLock::ScopedLock() { }

pMR::thread::ScopedLock::~ScopedLock() { }

bool pMR::thread::isThreaded()
{
    return true;
}

bool pMR::thread::isSerialized()
{
    return true;
}
