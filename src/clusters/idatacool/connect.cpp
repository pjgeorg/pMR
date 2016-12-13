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

#include "connection.hpp"
#include "target.hpp"
#include "../../backends/backend.hpp"
#include "../../misc/singleton.hpp"
#include "../../providers/verbs/topology.hpp"
#include "node.hpp"

void pMR::Connection::connect(Target const &target)
{
    if(target.isNull())
    {
        connectNull(target);
        return;
    }

    if(target.isSelf())
    {
        connectSelf(target);
        return;
    }

    auto originNode = Singleton<Node>::Instance();
    decltype(originNode) targetNode;

    backend::exchange(target, originNode, targetNode);

    if(originNode.getHostID() == targetNode.getHostID())
    {
        connectCMA(target);
    }
    else
    {
        connectVerbs(target, verbs::getIBAdapter());
    }
}
