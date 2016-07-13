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

#ifndef pMR_CLUSTERS_IDATACOOL_CONNECTION_H
#define pMR_CLUSTERS_IDATACOOL_CONNECTION_H

#include <memory>
#include <cstdint>
#include "provider.hpp"

namespace pMR
{
    class SendMemoryWindow;
    class RecvMemoryWindow;
    class Target;

    class Connection
    {
        friend SendMemoryWindow;
        friend RecvMemoryWindow;
        public:
            Connection(Target const &target);
            Connection(const Connection&) = delete;
            Connection(Connection&&) = default;
            Connection& operator=(const Connection&) = delete;
            Connection& operator=(Connection&&) = default;
            ~Connection() = default;
        private:
            Provider mProvider = Provider::null;
            std::shared_ptr<null::Connection> mNull;
            std::shared_ptr<self::Connection> mSelf;
            std::shared_ptr<loop::Connection> mLoop;
            std::shared_ptr<cma::Connection> mCMA;
            std::shared_ptr<verbs::Connection> mVerbs;
            void setProvider(Target const &target);
            Provider detectProvider(Target const &target) const;
            void connect(Target const &target);
            void connectNull(Target const &target);
            void connectSelf(Target const &target);
            void connectLoop(Target const &target);
            void connectCMA(Target const &target);
            void connectVerbs(Target const &target);
    };
}
#endif // pMR_CLUSTERS_IDATACOOL_CONNECTION_H
