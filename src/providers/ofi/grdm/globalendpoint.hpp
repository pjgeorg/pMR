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

#ifndef pMR_PROVIDERS_OFI_GRDM_GLOBALENDPOINT_H
#define pMR_PROVIDERS_OFI_GRDM_GLOBALENDPOINT_H

#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <config.hpp>
#include "../common/addressvector.hpp"
#include "../common/domain.hpp"
#include "../common/endpoint.hpp"
#include "../common/fabric.hpp"
#include "../common/info.hpp"
#include "../common/rma.hpp"
#include "../common/tagged.hpp"

namespace pMR
{
    namespace OFI
    {
        class GlobalEndpoint
        {
        public:
            GlobalEndpoint(Info &info);
            GlobalEndpoint(GlobalEndpoint const &) = delete;
            GlobalEndpoint(GlobalEndpoint &&) = delete;
            GlobalEndpoint &operator=(GlobalEndpoint const &) = delete;
            GlobalEndpoint &operator=(GlobalEndpoint &&) = delete;
            ~GlobalEndpoint() = default;
            fid_ep *get();
            fid_ep const *get() const;
            Domain &getDomain();
            Domain const &getDomain() const;

            std::vector<std::uint8_t> getAddress() const;
            fi_addr_t addPeer(std::vector<std::uint8_t> const &addr);

            void bind(std::uint64_t const sendID, std::uint64_t const recvID);
            void unbind(std::uint64_t const sendID, std::uint64_t const recvID);

            void postSend(Tagged &message);
            void postRecv(Tagged &message);
            void postWrite(RMA &message);

            void pollSend(std::uint64_t const iD);
            void pollRecv(std::uint64_t const iD);

            void checkMessageSize(std::size_t const size) const;

        private:
            Fabric mFabric;
            Domain mDomain;
            Endpoint mEndpoint;
            AddressVector mAddressVector;

            CompletionQueueContext mSendCompletionQueue;
#if defined OFI_RMA && !defined OFI_RMA_EVENT
            CompletionQueueData mRecvCompletionQueue;
#else
            CompletionQueueContext mRecvCompletionQueue;
#endif // OFI_RMA && !OFI_RMA_EVENT

            std::unordered_map<std::uint64_t, int> mSendCompletions;
            std::unordered_map<std::uint64_t, int> mRecvCompletions;

#ifndef OFI_GRDM_GLOBAL_THREADSAFE
            std::mutex mRecvMutex;
            std::mutex mSendMutex;
#endif // !OFI_GRDM_GLOBAL_THREADSAFE
            std::mutex mSendCompletionQueueMutex;
            std::mutex mRecvCompletionQueueMutex;
            std::mutex mSendCompletionsMutex;
            std::mutex mRecvCompletionsMutex;

            std::size_t mMaxSize = 0;
            std::size_t mInjectSize = 0;

            void bind(std::unordered_map<std::uint64_t, int> &map,
                std::uint64_t const iD);
            void unbind(std::unordered_map<std::uint64_t, int> &map,
                std::uint64_t const iD);

            bool checkCompletions(std::unordered_map<std::uint64_t, int> &map,
                std::uint64_t const iD);
            std::uint64_t retrieveCompletions(CompletionQueueContext &queue);
            std::uint64_t retrieveCompletions(CompletionQueueData &queue);

            template<typename T>
            void poll(T &queue, std::mutex &mutexQueue,
                std::unordered_map<std::uint64_t, int> &map,
                std::mutex &mutexMap, std::uint64_t const iD);
        };
    }
}

template<typename T>
void pMR::OFI::GlobalEndpoint::poll(T &queue, std::mutex &mutexQueue,
    std::unordered_map<std::uint64_t, int> &map, std::mutex &mutexMap,
    std::uint64_t const iD)
{
    if(cThreadLevel >= ThreadLevel::Multiple)
    {
        while(true)
        {
            if(mutexQueue.try_lock())
            {
                {
                    std::lock_guard<std::mutex> lock(mutexMap);
                    if(checkCompletions(map, iD))
                    {
                        mutexQueue.unlock();
                        return;
                    }
                }

                while(true)
                {
                    auto rID = decltype(iD){retrieveCompletions(queue)};

                    if(rID == iD)
                    {
                        mutexQueue.unlock();
                        return;
                    }

                    {
                        std::lock_guard<std::mutex> lock(mutexMap);
                        try
                        {
                            ++map.at({rID});
                        }
                        catch(std::exception const &e)
                        {
                            throw std::runtime_error(
                                "pMR: Retrieved unknown ID.");
                        }
                    }
                }
            }
            else
            {
                std::lock_guard<std::mutex> lock(mutexMap);
                if(checkCompletions(map, iD))
                {
                    return;
                }
            }
        }
    }
    else
    {
        if(checkCompletions(map, iD))
        {
            return;
        }

        while(true)
        {
            auto rID = decltype(iD){retrieveCompletions(queue)};

            if(rID == iD)
            {
                break;
            }

            try
            {
                ++map.at({rID});
            }
            catch(std::exception const &e)
            {
                throw std::runtime_error("pMR: Retrieved unknown ID.");
            }
        }
    }
}
#endif // pMR_PROVIDERS_OFI_GRDM_GLOBALENDPOINT_H
