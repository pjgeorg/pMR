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

#include "eventqueue.hpp"
#include <stdexcept>
#include "../ofi.hpp"

pMR::OFI::EventQueue::EventQueue(Fabric &fabric)
{
    fi_eq_attr attr = {};
    attr.size = {cEventQueueSize};
    attr.wait_obj = FI_WAIT_UNSPEC;

    if(fi_eq_open(fabric.get(), &attr, &mEventQueue, &mContext))
    {
        throw std::runtime_error("pMR: Unable to open event queue.");
    }
}

pMR::OFI::EventQueue::~EventQueue()
{
    if(mEventQueue)
    {
        fi_close(&mEventQueue->fid);
    }
}

fid_eq *pMR::OFI::EventQueue::get()
{
    return mEventQueue;
}

fid_eq const *pMR::OFI::EventQueue::get() const
{
    return mEventQueue;
}

pMR::OFI::Info pMR::OFI::EventQueue::pollConnectionRequest() const
{
    fi_eq_cm_entry entry = {};

    auto const event = poll(entry);

    if(event != FI_CONNREQ)
    {
        throw std::logic_error("pMR: Retrieved unexpected event.");
    }

    return Info(entry.info);
}

pMR::OFI::Info pMR::OFI::EventQueue::pollConnected() const
{
    fi_eq_cm_entry entry = {};

    auto const event = poll(entry);

    if(event != FI_CONNECTED)
    {
        throw std::logic_error("pMR: Retrieved unexpected event.");
    }

    return Info(entry.info);
}

std::uint32_t pMR::OFI::EventQueue::poll(fi_eq_cm_entry &entry) const
{
    std::uint32_t event;
    if(fi_eq_sread(mEventQueue, &event, static_cast<void *>(&entry),
           {sizeof(entry)}, -1, 0) != sizeof(entry))
    {
        throw std::runtime_error("pMR: Unable to retrieve event.");
    }

    return event;
}
