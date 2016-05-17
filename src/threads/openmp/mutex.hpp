#ifndef pMR_THREADS_OPENMP_MUTEX_H
#define pMR_THREADS_OPENMP_MUTEX_H

#include "../thread.hpp"
extern "C"
{
#include <omp.h>
}

namespace pMR { namespace thread
{
    class Mutex
    {
        public:
            Mutex();
            Mutex(const Mutex&) = delete;
            Mutex(Mutex&&) = delete;
            Mutex& operator=(const Mutex&) = delete;
            Mutex& operator=(Mutex&&) = delete;
            ~Mutex();
            void lock();
            void unlock();
        private:
            omp_lock_t mLock;
    };
}}
#endif // pMR_THREADS_OPENMP_MUTEX_H
