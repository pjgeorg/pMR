#include "memorywindow.hpp"
#include <cstring>
#include <stdexcept>
#include "connection.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT

pMR::self::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection), mBuffer(buffer), mSizeByte(sizeByte) { }

pMR::self::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection), mBuffer(buffer), mSizeByte(sizeByte) { }

void pMR::self::SendMemoryWindow::init() { }

void pMR::self::SendMemoryWindow::post()
{
    auto vDestinationBuffer =
        reinterpret_cast<void *volatile*>(&mConnection->mDestinationBuffer);
    while(*vDestinationBuffer == nullptr) { }

    if(mConnection->mDestinationSizeByte < mSizeByte)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
#ifdef HINT
    print("pMR: Hint: Using self communication. Intentional?");
    if(mConnection->mDestinationSizeByte > mSizeByte)
    {
        print("pMR: HINT: RecvWindow larger than SendWindow.");
    }
#endif // HINT

    std::memcpy(mConnection->mDestinationBuffer, mBuffer, mSizeByte);

    mConnection->mDestinationBuffer = nullptr;
}

void pMR::self::SendMemoryWindow::wait() { }

void pMR::self::RecvMemoryWindow::init()
{
    *mConnection->mRemoteBuffer = mBuffer;
    *mConnection->mRemoteSizeByte = mSizeByte;
}

void pMR::self::RecvMemoryWindow::post() { }

void pMR::self::RecvMemoryWindow::wait()
{
    auto vRemoteBuffer =
        reinterpret_cast<void *volatile*>(mConnection->mRemoteBuffer);
    while(*vRemoteBuffer != nullptr) { }
}
