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

#include "matchedqueue.hpp"
#include <stdexcept>
#include "../../misc/print.hpp"
#include "error.hpp"
#include "options.hpp"
#include "psm2.hpp"
#include "uuid.hpp"

pMR::PSM2::MatchedQueue::MatchedQueue(Endpoint &endpoint)
{
    auto err = psm2_mq_init(
        endpoint.get(), PSM2_MQ_ORDERMASK_NONE, NULL, 0, &mMatchedQueue);
    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to open MatchedQueue: " + toString(err));
    }
}

pMR::PSM2::MatchedQueue::~MatchedQueue()
{
    if(psm2_mq_finalize(mMatchedQueue))
    {
        print("pMR: Probably unable to finalize Matched Queue.");
    }
}

psm2_mq_t pMR::PSM2::MatchedQueue::get() const
{
    return mMatchedQueue;
}
