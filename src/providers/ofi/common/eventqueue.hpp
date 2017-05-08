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

#ifndef pMR_PROVIDERS_OFI_COMMON_EVENTQUEUE_H
#define pMR_PROVIDERS_OFI_COMMON_EVENTQUEUE_H

extern "C" {
#include <rdma/fabric.h>
#include <rdma/fi_eq.h>
}
#include "fabric.hpp"
#include "info.hpp"

namespace pMR
{
    namespace OFI
    {
        class EventQueue
        {
        public:
            EventQueue(Fabric &fabric);
            EventQueue(EventQueue const &) = delete;
            EventQueue(EventQueue &&) = delete;
            EventQueue &operator=(EventQueue const &) = delete;
            EventQueue &operator=(EventQueue &&) = delete;
            ~EventQueue();
            fid_eq *get();
            fid_eq const *get() const;
            Info pollConnectionRequest() const;
            Info pollConnected() const;

        private:
            fid_eq *mEventQueue = nullptr;
            fi_context mContext = {};
            std::uint32_t poll(fi_eq_cm_entry &entry) const;
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_EVENTQUEUE_H
