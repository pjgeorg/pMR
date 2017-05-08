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

#ifndef pMR_MISC_PROFILING_H
#define pMR_MISC_PROFILING_H

#ifdef pMR_PROFILING
#include "time.hpp"
#define pMR_PROF_START(x) x -= pMR::getTimeInSeconds()
#define pMR_PROF_STOP(x) x += pMR::getTimeInSeconds()
#define pMR_PROF_START_THREAD(x) (threadID) ?: x -= pMR::getTimeInSeconds()
#define pMR_PROF_STOP_THREAD(x) (threadID) ?: x += pMR::getTimeInSeconds()
#define pMR_PROF_COUNT(x) ++x
#else
#define pMR_PROF_START(x)
#define pMR_PROF_STOP(x)
#define pMR_PROF_START_THREAD(x)
#define pMR_PROF_STOP_THREAD(x)
#define pMR_PROF_COUNT(x)
#endif // pMR_PROFILING

#endif // pMR_MISC_PROFILING_H
