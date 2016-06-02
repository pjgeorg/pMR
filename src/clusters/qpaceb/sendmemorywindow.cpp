#include "sendmemorywindow.hpp"
#include "connection.hpp"
#include "../../providers/null/memorywindow.hpp"
#include "../../providers/self/memorywindow.hpp"
#include "../../providers/loop/memorywindow.hpp"
#include "../../providers/verbs/memorywindow.hpp"

pMR::SendMemoryWindow::SendMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mProvider(connection.mProvider)
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull = std::unique_ptr<null::SendMemoryWindow,
                    null::SendMemoryWindowDeleter>(new null::SendMemoryWindow(
                                connection.mNull, buffer, sizeByte));
            break;
        }
        case Provider::self:
        {
            mSelf = std::unique_ptr<self::SendMemoryWindow,
                    self::SendMemoryWindowDeleter>(new self::SendMemoryWindow(
                                connection.mSelf, buffer, sizeByte));
            break;
        }
        case Provider::loop:
        {
            mLoop = std::unique_ptr<loop::SendMemoryWindow,
                    loop::SendMemoryWindowDeleter>(new loop::SendMemoryWindow(
                                connection.mLoop, buffer, sizeByte));
            break;
        }
        case Provider::verbs:
        {
            mVerbs = std::unique_ptr<verbs::SendMemoryWindow,
                    verbs::SendMemoryWindowDeleter>(new verbs::SendMemoryWindow(
                                connection.mVerbs, buffer, sizeByte));
            break;
        }
    }
}

pMR::SendMemoryWindow::~SendMemoryWindow() { }

void pMR::SendMemoryWindow::init()
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

void pMR::SendMemoryWindow::post()
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

void pMR::SendMemoryWindow::wait()
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
