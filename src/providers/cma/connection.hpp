#ifndef pMR_PROVIDERS_CMA_CONNECTION_H
#define pMR_PROVIDERS_CMA_CONNECTION_H

#include <cstdint>
extern "C"
{
#include <sys/uio.h>
}

namespace pMR
{
    class Target;

    namespace cma
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
                void sendAddress(iovec &buffer);
                void sendData(iovec buffer, std::uint32_t const sizeByte);
                void postNotifySend() const;
                void pollNotifySend();
                void postNotifyRecv() const;
                void pollNotifyRecv();
            private:
                pid_t mRemotePID;
                iovec mRemoteAddress;
                iovec mDestination;
                iovec mRemoteNotifySend;
                bool mNotifySend = false;
                iovec mRemoteNotifyRecv;
                bool mNotifyRecv = false;
                void postNotify(iovec const&) const;
                void pollNotify(bool&);
                void checkBufferSize(iovec const &buffer);
        };
    }
}
#endif // pMR_PROVIDERS_CMA_CONNECTION_H
