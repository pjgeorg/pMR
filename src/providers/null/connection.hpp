#ifndef pMR_PROVIDERS_NULL_CONNECTION_H
#define pMR_PROVIDERS_NULL_CONNECTION_H

namespace pMR
{
    class Target;

    namespace null
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
                void initFence();
                void waitFence();
        };
    }
}
#endif // pMR_PROVIDERS_NULL_CONNECTION_H
