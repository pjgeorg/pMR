#include "connection.hpp"
#include "target.hpp"
#include "../../providers/null/connection.hpp"
#include "../../providers/self/connection.hpp"
#include "../../providers/loop/connection.hpp"
#include "../../providers/verbs/connection.hpp"

pMR::Connection::Connection(Target const &target)
{
    setProvider(target);
    connect(target);
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

    mProvider = Provider::verbs;
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
