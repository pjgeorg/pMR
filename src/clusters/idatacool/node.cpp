#include "node.hpp"

pMR::Node::Node()
    :   mHostID(gethostid()) { }

decltype(gethostid()) pMR::Node::getHostID() const
{
    return mHostID;
}
