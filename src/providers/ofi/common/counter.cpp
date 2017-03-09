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

#include "counter.hpp"
#include <stdexcept>
#include "../../arch/processor.hpp"

pMR::OFI::Counter::Counter(Domain &domain)
{
    fi_cntr_attr attr = {};

    attr.events = FI_CNTR_EVENTS_COMP;
    attr.wait_obj = FI_WAIT_UNSPEC;
    attr.flags = 0;

    if(fi_cntr_open(domain.get(), &attr, &mCounter, &mContext))
    {
        throw std::runtime_error("pMR: Unable to create counter.");
    }
}

pMR::OFI::Counter::~Counter()
{
    if(mCounter)
    {
        fi_close(&mCounter->fid);
    }
}

fid_cntr *pMR::OFI::Counter::get()
{
    return mCounter;
}

fid_cntr const *pMR::OFI::Counter::get() const
{
    return mCounter;
}

void pMR::OFI::Counter::poll()
{
#ifdef OFI_POLL_SPIN
    while(fi_cntr_read(mCounter) != 1)
    {
        CPURelax();
    }
#else
    if(fi_cntr_wait(mCounter, 1, -1))
    {
        throw std::runtime_error("pMR: Error waiting on counter.");
    }
#endif // OFI_POLL_SPIN

    if(fi_cntr_set(mCounter, 0))
    {
        throw std::runtime_error("pMR: Unable to set counter.");
    }
}
