#ifndef pMR_CLUSTERS_IDATACOOL_PROVIDER_H
#define pMR_CLUSTERS_IDATACOOL_PROVIDER_H

namespace pMR
{
    enum class Provider
    {
        null,
        self,
        loop,
        cma,
        verbs
    };

    namespace null
    {
        class Connection;
        class SendMemoryWindow;
        class RecvMemoryWindow;
    }
    namespace self
    {
        class Connection;
        class SendMemoryWindow;
        class RecvMemoryWindow;
    }
    namespace loop
    {
        class Connection;
        class SendMemoryWindow;
        class RecvMemoryWindow;
    }
    namespace cma
    {
        class Connection;
        class SendMemoryWindow;
        class RecvMemoryWindow;
    }
    namespace verbs
    {
        class Connection;
        class SendMemoryWindow;
        class RecvMemoryWindow;
    }
}
#endif // pMR_PROVIDERS_IDATACOOL_PROVIDER_H
