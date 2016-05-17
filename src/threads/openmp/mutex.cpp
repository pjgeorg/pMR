#include "mutex.hpp"

pMR::thread::Mutex::Mutex()
{
    omp_init_lock(&mLock);
}

pMR::thread::Mutex::~Mutex()
{
    omp_destroy_lock(&mLock);
}

void pMR::thread::Mutex::lock()
{
    omp_set_lock(&mLock);
}

void pMR::thread::Mutex::unlock()
{
    omp_unset_lock(&mLock);
}
