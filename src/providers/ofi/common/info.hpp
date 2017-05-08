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

#ifndef pMR_PROVIDERS_OFI_COMMON_INFO_H
#define pMR_PROVIDERS_OFI_COMMON_INFO_H

#include <cstdint>
#include <string>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}

namespace pMR
{
    namespace OFI
    {
        class Info
        {
        public:
            explicit Info(std::string const &address = std::string());
            explicit Info(fi_info *info);
            Info(Info const &);
            Info(Info &&);
            Info &operator=(Info const &) = delete;
            Info &operator=(Info &&) = delete;
            ~Info();
            fi_info *get();
            fi_info const *get() const;
            std::size_t numProviders() const;
            std::size_t getProvider() const;
            void setProvider(std::size_t num);
            std::string getName() const;
            std::string getProviderName() const;
            std::string getProtocol() const;
            std::string getStruct() const;
            bool checkProvider() const;
            void searchProvider(std::string const &providerName);
            fi_fabric_attr *getFabric();
            fi_fabric_attr const *getFabric() const;
            std::size_t injectSize() const;
            std::size_t maxSize() const;
            std::size_t getContextSize() const;
            std::size_t getTransmitContextSize() const;
            std::size_t getReceiveContextSize() const;
            void setSourceAddress(std::vector<std::uint8_t> const &address);
            void setDestinationAddress(
                std::vector<std::uint8_t> const &address);
            Info &operator++();
            Info &operator--();

        private:
            fi_info *mInfo = nullptr;
            std::vector<fi_info *> mInfos;
            std::size_t mProvider = 0;
            void queryStruct();
        };

        Info getProvider(std::string const &providerName,
            std::string const &address = std::string());
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_INFO_H
