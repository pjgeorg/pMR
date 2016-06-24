#ifndef pMR_CLUSTERS_QPACEB_RECVMEMORYWINDOW_H
#define pMR_CLUSTERS_QPACEB_RECVMEMORYWINDOW_H

#include <cstdint>
#include <memory>
#include "provider.hpp"

namespace pMR
{
    class Connection;

    class RecvMemoryWindow
    {
        public:
            RecvMemoryWindow(Connection const&,
                    void *buffer, std::uint32_t const sizeByte);
            RecvMemoryWindow(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow(RecvMemoryWindow&&) = default;
            RecvMemoryWindow& operator=(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow& operator=(RecvMemoryWindow&&) = default;
            ~RecvMemoryWindow();
            void init();
            void post();
            void wait();
            void* data();
            void const* data() const;
            std::uint32_t size() const;
        private:
            void *const mBuffer;
            std::uint32_t mSizeByte;
            Provider mProvider = Provider::null;
            std::unique_ptr<null::RecvMemoryWindow,
                null::RecvMemoryWindowDeleter> mNull;
            std::unique_ptr<self::RecvMemoryWindow,
                self::RecvMemoryWindowDeleter> mSelf;
            std::unique_ptr<loop::RecvMemoryWindow,
                loop::RecvMemoryWindowDeleter> mLoop;
            std::unique_ptr<verbs::RecvMemoryWindow,
                verbs::RecvMemoryWindowDeleter> mVerbs;
    };
}
#endif // pMR_CLUSTERS_QPACEB_RECVMEMORYWINDOW_H
