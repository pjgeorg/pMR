#ifndef pMR_PROVIDERS_SELF_CONNECTION_H
#define pMR_PROVIDERS_SELF_CONNECTION_H

#include <cstdint>

namespace pMR
{
    class Target;

    namespace self
    {
        class Connection
        {
            public:
                Connection(Target const &target);
                Connection(const Connection&) = delete;
                Connection(Connection&&) = delete;
                Connection& operator=(const Connection&) = delete;
                Connection& operator=(Connection&&) = delete;
                ~Connection() = default;
                void **mRemoteBuffer = nullptr;
                std::uint32_t *mRemoteSizeByte = nullptr;
                void *mDestinationBuffer = nullptr;
                std::uint32_t mDestinationSizeByte;
        };
    }
}
#endif // pMR_PROVIDERS_SELF_CONNECTION_H
