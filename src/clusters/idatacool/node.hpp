#ifndef pMR_CLUSTERS_IDATACOOL_NODE_H
#define pMR_CLUSTERS_IDATACOOL_NODE_H

extern "C"
{
#include <unistd.h>
}

namespace pMR
{
    class Node
    {
        public:
            Node();
            ~Node() = default;
            decltype(gethostid()) getHostID() const;
        private:
            decltype(gethostid()) mHostID;
    };
}
#endif // pMR_CLUSTERS_IDATACOOL_NODE_H
