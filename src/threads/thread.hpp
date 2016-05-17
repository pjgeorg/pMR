#ifndef pMR_THREADS_THREAD_H
#define pMR_THREADS_THREAD_H

#include <memory>

namespace pMR { namespace thread
{
    class Mutex;
    class ScopedLock
    {
        public:
            ScopedLock();
            ScopedLock(const ScopedLock&) = delete;
            ScopedLock(ScopedLock&&) = delete;
            ScopedLock& operator=(const ScopedLock&) = delete;
            ScopedLock& operator=(ScopedLock&&) = delete;
            ~ScopedLock();
        private:
            static Mutex sMutex;
    };

    bool isMT();
}}
#endif // pMR_THREADS_THREAD_H
