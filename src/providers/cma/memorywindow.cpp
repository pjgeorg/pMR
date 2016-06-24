#include "memorywindow.hpp"
#include "connection.hpp"

#include "../../misc/print.hpp"

pMR::cma::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection)
{
    mBuffer.iov_base = buffer;
    mBuffer.iov_len = sizeByte;
}

pMR::cma::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mConnection(connection)
{
    mBuffer.iov_base = buffer;
    mBuffer.iov_len = sizeByte;
}

void pMR::cma::SendMemoryWindow::init() { }

void pMR::cma::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    mConnection->pollNotifySend();
    mConnection->sendData(mBuffer, sizeByte);
    mConnection->postNotifyRecv();
}

void pMR::cma::SendMemoryWindow::wait() { }

void pMR::cma::RecvMemoryWindow::init()
{
    mConnection->sendAddress(mBuffer);
    mConnection->postNotifySend();
}

void pMR::cma::RecvMemoryWindow::post() { }

void pMR::cma::RecvMemoryWindow::wait()
{
    mConnection->pollNotifyRecv();
}
