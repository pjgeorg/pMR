#include "connection.hpp"
#include "node.hpp"
#include "../../providers/verbs/connection.hpp"
#include "../../providers/verbs/topology.hpp"
#include "../../misc/singleton.hpp"
#include "../../backends/backend.hpp"

void pMR::Connection::connectVerbs(Target const &target)
{
    verbs::Devices devices;
    auto device = verbs::getIBAdapter(devices);

    mVerbs = std::make_shared<verbs::Connection>(target, device);
}

pMR::Provider pMR::Connection::detectProvider(Target const &target) const
{
    auto originNode = Singleton<Node>::Instance();
    decltype(originNode) targetNode;

    backend::exchange(target, originNode, targetNode);
    
    if(originNode.getHostID() == targetNode.getHostID())
    {
        return Provider::cma;
    }
    else
    {
        return Provider::verbs;
    }
}
