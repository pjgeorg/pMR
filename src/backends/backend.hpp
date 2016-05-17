#ifndef pMR_BACKEND_H
#define pMR_BACKEND_H

#include <array>
#include <cstdint>

namespace pMR
{
    class Target;

    namespace backend
    {
        void sync(Target const &target);
    
        void exchange(Target const &target,
                void *sendBuffer, void *recvBuffer,
                std::uint32_t const sizeByte);
    
        template<typename T>
        void exchange(Target const &target, T &sendBuffer, T &recvBuffer)
        {
            return exchange(target, static_cast<void*>(&sendBuffer),
                    static_cast<void*>(&recvBuffer), sizeof(sendBuffer));
        }

        template<typename T, std::size_t N>
        void exchange(Target const &target,
                std::array<T, N> &sendBuffer, std::array<T, N> &recvBuffer)
        {
            return exchange(target, sendBuffer.data(),
                    recvBuffer.data(), sizeof(sendBuffer));
        }
    }
}
#endif // pMR_BACKEND_H
