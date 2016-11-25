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

#include "completionqueue.hpp"
#include <stdexcept>
#include "../../misc/string.hpp"
#include "../../arch/processor.hpp"

pMR::verbs::CompletionQueue::CompletionQueue(Context &context, int const size)
{
    mCompletionQueue = ibv_create_cq(context.get(), {size}, NULL, NULL, 0);

    if(!mCompletionQueue)
    {
        throw std::runtime_error("pMR: Could not create Completion Queue.");
    }
}

pMR::verbs::CompletionQueue::~CompletionQueue()
{
    ibv_destroy_cq(mCompletionQueue);
}

ibv_cq* pMR::verbs::CompletionQueue::get()
{
    return mCompletionQueue;
}

ibv_cq const* pMR::verbs::CompletionQueue::get() const
{
    return mCompletionQueue;
}

void pMR::verbs::CompletionQueue::poll()
{
    ibv_wc workCompletion;
    int numCompletion;
    do
    {
        numCompletion = ibv_poll_cq(mCompletionQueue, 1, &workCompletion);
        CPURelax();
    }
    while(numCompletion == 0);

    if(numCompletion < 0)
    {
        throw std::runtime_error("pMR: Failed to poll CQ.");
    }

    if(workCompletion.status != IBV_WC_SUCCESS)
    {
        throw std::runtime_error(toString("pMR: Work Request ID",
                    workCompletion.wr_id, "failed with status:",
                    ibv_wc_status_str(workCompletion.status)));
    }
}

bool pMR::verbs::CompletionQueue::poll(int retry)
{
    ibv_wc workCompletion;
    int numCompletion;
    do
    {
        numCompletion = ibv_poll_cq(mCompletionQueue, 1, &workCompletion);
        CPURelax();
    }
    while(numCompletion == 0 && --retry);

    if(numCompletion < 0)
    {
        throw std::runtime_error("pMR: Failed to poll CQ.");
    }

    if(workCompletion.status != IBV_WC_SUCCESS)
    {
        throw std::runtime_error(toString("pMR: Work Request ID",
                    workCompletion.wr_id, "failed with status:",
                    ibv_wc_status_str(workCompletion.status)));
    }

    if(numCompletion)
    {
        return {true};
    }
    else
    {
        return {false};
    }
}
