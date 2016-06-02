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
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
    namespace self
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
    namespace loop
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
    namespace verbs
    {
        class Connection;
        class SendMemoryWindow;
        class SendMemoryWindowDeleter
        {
            public:
                void operator()(SendMemoryWindow*) const;
        };
        class RecvMemoryWindow;
        class RecvMemoryWindowDeleter
        {
            public:
                void operator()(RecvMemoryWindow*) const;
        };
    }
}
#endif // pMR_PROVIDERS_QPACEB_PROVIDER_H
