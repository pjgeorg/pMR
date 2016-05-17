#include "completionqueue.hpp"
#include <stdexcept>
#include "../../misc/string.hpp"

pMR::verbs::CompletionQueue::CompletionQueue(Context &context, int const size)
{
    mCompletionQueue = ibv_create_cq(context.get(), size, NULL, NULL, 0);

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
    int received = 0;
    while(received <= 0)
    {
        received = ibv_poll_cq(mCompletionQueue, 1, &workCompletion);
    }
    if(workCompletion.status != IBV_WC_SUCCESS)
    {
        throw std::runtime_error(toString("pMR: Completion Queue ID",
                    workCompletion.wr_id, "failed with status",
                    workCompletion.status));
    }
}

bool pMR::verbs::CompletionQueue::poll(int count)
{
    ibv_wc workCompletion;
    int returnValue = 0;
    do
    {
        if((returnValue = ibv_poll_cq(mCompletionQueue, 1, &workCompletion)))
        {
            if(returnValue < 0 || workCompletion.status != IBV_WC_SUCCESS)
            {
                throw std::runtime_error(toString("pMR: Completion Queue ID",
                            workCompletion.wr_id, "failed with status",
                            workCompletion.status));
            }
            return true;
        }
    }
    while(--count);
    return false;
}
