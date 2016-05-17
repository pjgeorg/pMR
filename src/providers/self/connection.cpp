#include "connection.hpp"
#include <array>
#include "../../backends/backend.hpp"

pMR::self::Connection::Connection(Target const &target)
{
    std::array<std::uintptr_t, 2> originAddress, targetAddress;

    std::get<0>(originAddress) =
        reinterpret_cast<std::uintptr_t>(&mDestinationBuffer);
    std::get<1>(originAddress) =
        reinterpret_cast<std::uintptr_t>(&mDestinationSizeByte);

    backend::exchange(target, originAddress, targetAddress);

    mRemoteBuffer =
        reinterpret_cast<void**>(std::get<0>(targetAddress));
    mRemoteSizeByte =
        reinterpret_cast<std::uint32_t*>(std::get<1>(targetAddress));
}

void pMR::self::Connection::initFence()
{
    *mRemoteBuffer = &mDestinationBuffer;
}

void pMR::self::Connection::waitFence()
{
    auto vDestinationBuffer =
        reinterpret_cast<void *volatile*>(&mDestinationBuffer);
    while(*vDestinationBuffer == nullptr) { }
    mDestinationBuffer = nullptr;
}
