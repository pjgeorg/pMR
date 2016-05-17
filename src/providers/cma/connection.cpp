#include "connection.hpp"
#include <array>
#include <stdexcept>
extern "C"
{
#include <unistd.h>
}
#include "../../backends/backend.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT

pMR::cma::Connection::Connection(Target const &target)
{
    std::array<std::uint64_t, 7> originAddress, targetAddress;

    std::get<0>(originAddress) = getpid();
    std::get<1>(originAddress) =
        reinterpret_cast<std::uintptr_t>(&mDestination);
    std::get<2>(originAddress) = sizeof(mDestination);
    std::get<3>(originAddress) = reinterpret_cast<std::uintptr_t>(&mNotifySend);
    std::get<4>(originAddress) = sizeof(mNotifySend);
    std::get<5>(originAddress) = reinterpret_cast<std::uintptr_t>(&mNotifyRecv);
    std::get<6>(originAddress) = sizeof(mNotifyRecv);

    backend::exchange(target, originAddress, targetAddress);

    mRemotePID = std::get<0>(targetAddress);
    mRemoteAddress.iov_base =
        reinterpret_cast<void*>(std::get<1>(targetAddress));
    mRemoteAddress.iov_len =
        static_cast<std::size_t>(std::get<2>(targetAddress));
    mRemoteNotifySend.iov_base = 
        reinterpret_cast<void*>(std::get<3>(targetAddress));
    mRemoteNotifySend.iov_len =
        static_cast<std::size_t>(std::get<4>(targetAddress));
    mRemoteNotifyRecv.iov_base = 
        reinterpret_cast<void*>(std::get<5>(targetAddress));
    mRemoteNotifyRecv.iov_len =
        static_cast<std::size_t>(std::get<6>(targetAddress));
}

void pMR::cma::Connection::initFence()
{
    postNotifySend();
}

void pMR::cma::Connection::waitFence()
{
    pollNotifySend();
}

void pMR::cma::Connection::sendAddress(iovec &buffer)
{
    iovec originAddress;
    originAddress.iov_base = &buffer;
    originAddress.iov_len = sizeof(buffer);

    auto ret = process_vm_writev(mRemotePID, &originAddress, 1,
            &mRemoteAddress, 1, 0);

    if(ret < 0 || static_cast<std::size_t>(ret) != originAddress.iov_len)
    {
        throw std::runtime_error("pMR: CMA failed to send address.");
    }
}

void pMR::cma::Connection::sendData(iovec const &buffer)
{
    checkBufferSize(buffer);

    auto vDestination = reinterpret_cast<iovec volatile *>(&mDestination);

    auto ret = process_vm_writev(mRemotePID, &buffer, 1,
            const_cast<iovec*>(vDestination), 1, 0);

    if(ret < 0 || static_cast<std::size_t>(ret) != buffer.iov_len)
    {
        throw std::runtime_error("pMR: CMA failed to write data.");
    }
}

void pMR::cma::Connection::postNotifySend() const
{
    postNotify(mRemoteNotifySend);
}

void pMR::cma::Connection::pollNotifySend()
{
    pollNotify(mNotifySend);
}

void pMR::cma::Connection::postNotifyRecv() const
{
    postNotify(mRemoteNotifyRecv);
}

void pMR::cma::Connection::pollNotifyRecv()
{
    pollNotify(mNotifyRecv);
}

void pMR::cma::Connection::postNotify(iovec const &remoteNotify) const
{
    bool notify = true;
    iovec localNotify;
    localNotify.iov_base = &notify;
    localNotify.iov_len = sizeof(notify);

    auto ret = process_vm_writev(mRemotePID, &localNotify, 1,
            &remoteNotify, 1, 0);

    if(ret < 0 || static_cast<std::size_t>(ret) != localNotify.iov_len)
    {
        throw std::runtime_error("pMR: CMA failed to send notification.");
    }
}

void pMR::cma::Connection::pollNotify(bool &notify)
{
    auto vNotify = reinterpret_cast<bool volatile*>(&notify);
    while(*vNotify == false) { }
    notify = false;
}

void pMR::cma::Connection::checkBufferSize(iovec const &buffer)
{
    if(mDestination.iov_len < buffer.iov_len)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }

#ifdef HINT
    if(mDestination.iov_len > buffer.iov_len)
    {
        print("pMR: HINT: RecvWindow larger than SendWindow.");
    }
#endif // HINT
}
