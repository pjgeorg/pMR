#ifndef pMR_CLUSTERS_QPACEB_PROVIDER_H
#define pMR_CLUSTERS_QPACEB_PROVIDER_H

namespace pMR
{
    enum class Provider
    {
        null,
        self,
        loop,
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
    namespace verbs
    {
        class Connection;
        class SendMemoryWindow;
        class RecvMemoryWindow;
    }
}
#endif // pMR_PROVIDERS_QPACEB_PROVIDER_H
