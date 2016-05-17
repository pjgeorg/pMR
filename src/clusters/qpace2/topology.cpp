#include "connection.hpp"
#include "topology.hpp"
#include "node.hpp"
#include "../../providers/verbs/connection.hpp"
#include "../../providers/verbs/topology.hpp"
#include "../../misc/singleton.hpp"
#include "../../backends/backend.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT

void pMR::Connection::connectVerbs(Target const &target)
{
    verbs::Devices devices;
    auto device = verbs::getIBAdapter(devices);

    auto originNode = Singleton<Node>::Instance(device);

    auto sendBuffer = originNode.flatten();
    decltype(sendBuffer) recvBuffer;

    backend::exchange(target, sendBuffer, recvBuffer); 

    decltype(originNode) targetNode(recvBuffer);

    auto portNumber = detectBestPort(originNode, targetNode);

    mVerbs = std::make_shared<verbs::Connection>(target, device, portNumber);
}

std::uint8_t pMR::detectBestPort(Node const &origin, Node const &target)
{
    // Origin and Target on same brick
    if(origin.getNodeGUID() == target.getNodeGUID())
    {
        if(origin.getSCIFNodeID() + target.getSCIFNodeID() != 5)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    // Origin and Target in same hyperblock
    if(origin.getSwitchLID(1) == target.getSwitchLID(1) &&
            origin.getSwitchLID(2) == target.getSwitchLID(2))
    {
        if(origin.getSCIFNodeID() + target.getSCIFNodeID() < 6)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    // At least one common switch
    for(std::uint8_t portNumber = 1; portNumber != 3; ++portNumber)
    {
        if(origin.getSwitchLID(portNumber) == target.getSwitchLID(portNumber))
        {
            return portNumber;
        }
    }

    // No common switch
#ifdef HINT
    print("pMR: HINT: Using bad path. Check topology.");
#endif // HINT
    if(origin.getSCIFNodeID() + target.getSCIFNodeID() < 6)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
