#include "recvmemorywindow.hpp"
#include "connection.hpp"
#include "../../providers/null/memorywindow.hpp"
#include "../../providers/self/memorywindow.hpp"
#include "../../providers/loop/memorywindow.hpp"
#include "../../providers/verbs/memorywindow.hpp"

pMR::RecvMemoryWindow::RecvMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mProvider(connection.mProvider)
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull = std::unique_ptr<null::RecvMemoryWindow,
                    null::RecvMemoryWindowDeleter>(new null::RecvMemoryWindow(
                                connection.mNull, buffer, sizeByte));
            break;
        }
        case Provider::self:
        {
            mSelf = std::unique_ptr<self::RecvMemoryWindow,
                    self::RecvMemoryWindowDeleter>(new self::RecvMemoryWindow(
                                connection.mSelf, buffer, sizeByte));
            break;
        }
        case Provider::loop:
        {
            mLoop = std::unique_ptr<loop::RecvMemoryWindow,
                    loop::RecvMemoryWindowDeleter>(new loop::RecvMemoryWindow(
                                connection.mLoop, buffer, sizeByte));
            break;
        }
        case Provider::verbs:
        {
            mVerbs = std::unique_ptr<verbs::RecvMemoryWindow,
                    verbs::RecvMemoryWindowDeleter>(new verbs::RecvMemoryWindow(
                                connection.mVerbs, buffer, sizeByte));
            break;
        }
    }
}

pMR::RecvMemoryWindow::~RecvMemoryWindow() { }

void pMR::RecvMemoryWindow::init()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->init();
            break;
        }
        case Provider::self:
        {
            mSelf->init();
            break;
        }
        case Provider::loop:
        {
            mLoop->init();
            break;
        }
        case Provider::verbs:
        {
            mVerbs->init();
            break;
        }
    }
}

void pMR::RecvMemoryWindow::post()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->post();
            break;
        }
        case Provider::self:
        {
            mSelf->post();
            break;
        }
        case Provider::loop:
        {
            mLoop->post();
            break;
        }
        case Provider::verbs:
        {
            mVerbs->post();
            break;
        }
    }
}

void pMR::RecvMemoryWindow::wait()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->wait();
            break;
        }
        case Provider::self:
        {
            mSelf->wait();
            break;
        }
        case Provider::loop:
        {
            mLoop->wait();
            break;
        }
        case Provider::verbs:
        {
            mVerbs->wait();
            break;
        }
    }
}
