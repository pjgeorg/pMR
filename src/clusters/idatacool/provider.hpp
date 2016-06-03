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
    namespace cma
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
#endif // pMR_CLUSTERS_IDATACOOL_PROVIDER_H
