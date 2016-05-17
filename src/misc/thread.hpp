#ifndef pMR_MISC_THREAD_H
#define pMR_MISC_THREAD_H

#include <stdexcept>
#include <algorithm>

namespace pMR
{
// ?!?!    void splitThreadsIntoGangs();
    template<typename T, typename W>
    void splitWorkToThreads(W const start, W const end, T const threadID,
            T const threadCount, W& threadStart, W& threadEnd,
            W const granularity = 1);
}

template<typename T, typename W>
void pMR::splitWorkToThreads(W const start, W const end, T const threadID,
        T const threadCount, W& threadStart, W& threadEnd, W const granularity)
{
    threadStart = start;
    threadEnd = end;
    if(end - start == 0)
    {
        return;
    }
    if(!threadCount > 0)
    {
        throw std::logic_error("Thread Count");
    }
    if(threadID >= threadCount || threadID < 0)
    {
        throw std::out_of_range("Thread ID");
    }

    W perThread = static_cast<W>(std::ceil(
                (static_cast<double>(end - start) / granularity)
                / threadCount) * granularity); 

    threadStart += threadID * perThread;
    threadEnd = threadStart;

    W usedThreads = (end - start) / perThread;

    if(threadID < usedThreads)
    {
        threadEnd += perThread;
    }
    else
    {
        if(threadID == usedThreads)
        {
            threadEnd += (end - start) - usedThreads * perThread;
        }
    }
}
#endif // pMR_MISC_THREAD_H
