#include "connection.hpp"
#include "target.hpp"
#include "../../providers/null/connection.hpp"
#include "../../providers/self/connection.hpp"
#include "../../providers/loop/connection.hpp"
#include "../../providers/cma/connection.hpp"
#include "../../providers/verbs/connection.hpp"

pMR::Connection::Connection(Target const &target)
{
    setProvider(target);
    connect(target);
}

void pMR::Connection::initFence()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->initFence();
            break;
        }
        case Provider::self:
        {
            mSelf->initFence();
            break;
        }
        case Provider::loop:
        {
            mLoop->initFence();
            break;
        }
        case Provider::cma:
        {
            mCMA->initFence();
            break;
        }
        case Provider::verbs:
        {
            mVerbs->initFence();
            break;
        }
    }
}

void pMR::Connection::waitFence()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->waitFence();
            break;
        }
        case Provider::self:
        {
            mSelf->waitFence();
            break;
        }
        case Provider::loop:
        {
            mLoop->waitFence();
            break;
        }
        case Provider::cma:
        {
            mCMA->waitFence();
            break;
        }
        case Provider::verbs:
        {
            mVerbs->waitFence();
            break;
        }
    }
}

void pMR::Connection::setProvider(Target const &target)
{
    if(target.isNull())
    {
        mProvider = Provider::null;
        return;
    }

    if(target.isSelf())
    {
        mProvider = Provider::self;
        return;
    }

    if(target.isLoop())
    {
        mProvider = Provider::loop;
        return;
    }

    mProvider = detectProvider(target);
}

void pMR::Connection::connect(Target const &target)
{
    switch(mProvider)
    {
        case Provider::null:
        {
            connectNull(target);
            break;
        }
        case Provider::self:
        {
            connectSelf(target);
            break;
        }
        case Provider::loop:
        {
            connectLoop(target);
            break;
        }
        case Provider::cma:
        {
            connectCMA(target);
            break;
        }
        case Provider::verbs:
        {
            connectVerbs(target);
            break;
        }
    }
}

void pMR::Connection::connectNull(Target const &target)
{
    mNull = std::make_shared<null::Connection>(target);
}

void pMR::Connection::connectSelf(Target const &target)
{
    mSelf = std::make_shared<self::Connection>(target);
}

void pMR::Connection::connectLoop(Target const &target)
{
    mLoop = std::make_shared<loop::Connection>(target);
}

void pMR::Connection::connectCMA(Target const &target)
{
    mCMA = std::make_shared<cma::Connection>(target);
}
