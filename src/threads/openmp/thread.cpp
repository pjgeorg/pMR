#include "../thread.hpp"
#include "mutex.hpp"

pMR::thread::ScopedLock::ScopedLock()
{
    sMutex.lock();
}

pMR::thread::ScopedLock::~ScopedLock()
{
    sMutex.unlock();
}

pMR::thread::Mutex pMR::thread::ScopedLock::sMutex;

bool pMR::thread::isMT()
{
    return true;
}
