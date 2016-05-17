#include "memorywindow.hpp"
#include <stdexcept>
#include "memoryaddress.hpp"
#include "connection.hpp"
#ifdef HINT
#include "../../misc/print.hpp"
#endif // HINT


pMR::verbs::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mMemoryRegion(mConnection->getContext(),
                mConnection->getProtectionDomain(),
                buffer, sizeByte, IBV_ACCESS_LOCAL_WRITE) { }

pMR::verbs::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection),
        mMemoryRegion(mConnection->getContext(),
                mConnection->getProtectionDomain(),
                buffer, sizeByte,
                IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE) { }

void pMR::verbs::SendMemoryWindow::init()
{
    mConnection->postRecvRequest();
}

void pMR::verbs::SendMemoryWindow::post()
{
    // Recv memory address from target 
    mConnection->pollRecvCompletionQueue();
    
    MemoryAddress remoteMemoryAddress = mConnection->getRemoteMemoryAddress();

    if(remoteMemoryAddress.getLength() < mMemoryRegion.getLength())
    {
        throw std::length_error("pMR: RecvWindow smaller than SendWindow.");
    }
#ifdef HINT
    if(remoteMemoryAddress.getLength() > mMemoryRegion.getLength())
    {
        print("pMR: HINT: RecvWindow larger than SendWindow.");
    }
#endif // HINT

    mConnection->postRDMAWriteRequest(mMemoryRegion,
            mConnection->getRemoteMemoryAddress());
}

void pMR::verbs::SendMemoryWindow::wait()
{
    mConnection->pollSendCompletionQueue();
}

void pMR::verbs::RecvMemoryWindow::init()
{
    mConnection->setLocalMemoryAddress(mMemoryRegion);
    // Send memory address to target
    mConnection->postSendRequest();
    mConnection->postRecvRequest();
}

void pMR::verbs::RecvMemoryWindow::post() { }

void pMR::verbs::RecvMemoryWindow::wait()
{
    mConnection->pollSendCompletionQueue();
    mConnection->pollRecvCompletionQueue();
}
