#include "connection.hpp"
#include "../../providers/verbs/connection.hpp"
#include "../../providers/verbs/topology.hpp"

void pMR::Connection::connectVerbs(Target const &target)
{
    verbs::Devices devices;
    auto device = verbs::getIBAdapter(devices);

    mVerbs = std::make_shared<verbs::Connection>(target, device);
}
