#include "connection.hpp"

pMR::loop::Connection::Connection(Target const&) { }

void pMR::loop::Connection::initFence()
{
    mDestinationBuffer = &mDestinationBuffer;
}

void pMR::loop::Connection::waitFence()
{
    auto vDestinationBuffer =
        reinterpret_cast<void *volatile*>(&mDestinationBuffer);
    while(*vDestinationBuffer == nullptr) { }
    mDestinationBuffer = nullptr;
}
