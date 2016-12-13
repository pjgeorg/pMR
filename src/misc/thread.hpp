//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef pMR_MISC_THREAD_H
#define pMR_MISC_THREAD_H

#include <algorithm>
#include <stdexcept>

namespace pMR
{
    // ?!?!    void splitThreadsIntoGangs();
    template<typename T, typename W>
    void splitWorkToThreads(W const start, W const end, T const threadID,
        T const threadCount, W &threadStart, W &threadEnd,
        W const granularity = 1);
}

template<typename T, typename W>
void pMR::splitWorkToThreads(W const start, W const end, T const threadID,
    T const threadCount, W &threadStart, W &threadEnd, W const granularity)
{
    threadStart = start;
    threadEnd = end;
    if(end - start == 0)
    {
        return;
    }
    if(!(threadCount > 0))
    {
        throw std::logic_error("Thread Count");
    }
    if(threadID >= threadCount || threadID < 0)
    {
        throw std::out_of_range("Thread ID");
    }

    W perThread = static_cast<W>(
        std::ceil(
            (static_cast<double>(end - start) / granularity) / threadCount) *
        granularity);

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
