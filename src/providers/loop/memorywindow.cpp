#include "memorywindow.hpp"
#include <stdexcept>
#include <cstring>
#include "connection.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT

pMR::loop::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection), mBuffer(buffer), mSizeByte(sizeByte) { }

pMR::loop::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection), mBuffer(buffer), mSizeByte(sizeByte) { }

void pMR::loop::SendMemoryWindow::init() { }

void pMR::loop::SendMemoryWindow::post()
{
    auto vDestinationBuffer =
        reinterpret_cast<void *volatile*>(&mConnection->mDestinationBuffer);
    while(*vDestinationBuffer == nullptr) { }

    if(mConnection->mDestinationSizeByte < mSizeByte)
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
#ifdef HINT
    print("pMR: Hint: Using loop communication. Intentional?");
    if(mConnection->mDestinationSizeByte > mSizeByte)
    {
        print("pMR: HINT: RecvWindow larger than SendWindow.");
    }
#endif // HINT

    std::memcpy(mConnection->mDestinationBuffer, mBuffer, mSizeByte);

    mConnection->mDestinationBuffer = nullptr;
}

void pMR::loop::SendMemoryWindow::wait() { }

void pMR::loop::RecvMemoryWindow::init()
{
    mConnection->mDestinationBuffer = mBuffer;
    mConnection->mDestinationSizeByte = mSizeByte;
}

void pMR::loop::RecvMemoryWindow::post() { }

void pMR::loop::RecvMemoryWindow::wait() { }
