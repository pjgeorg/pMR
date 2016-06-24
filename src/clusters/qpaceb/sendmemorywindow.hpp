#ifndef pMR_CLUSTERS_QPACEB_SENDMEMORYWINDOW_H
#define pMR_CLUSTERS_QPACEB_SENDMEMORYWINDOW_H

#include <cstdint>
#include <memory>
#include "provider.hpp"

namespace pMR
{
    class Connection;

    class SendMemoryWindow
    {
        public:
            SendMemoryWindow(Connection const&,
                    void *buffer, std::uint32_t const sizeByte);
            SendMemoryWindow(const SendMemoryWindow&) = delete;
            SendMemoryWindow(SendMemoryWindow&&) = default;
            SendMemoryWindow& operator=(const SendMemoryWindow&) = delete;
            SendMemoryWindow& operator=(SendMemoryWindow&&) = default;
            ~SendMemoryWindow();
            void init();
            void post();
            void post(std::uint32_t const sizeByte);
            void wait();
            void* data();
            void const* data() const;
            std::uint32_t size() const;
        private:
            void *const mBuffer;
            std::uint32_t mSizeByte;
            Provider mProvider = Provider::null;
            std::unique_ptr<null::SendMemoryWindow,
                null::SendMemoryWindowDeleter> mNull;
            std::unique_ptr<self::SendMemoryWindow,
                self::SendMemoryWindowDeleter> mSelf;
            std::unique_ptr<loop::SendMemoryWindow,
                loop::SendMemoryWindowDeleter> mLoop;
            std::unique_ptr<verbs::SendMemoryWindow,
                verbs::SendMemoryWindowDeleter> mVerbs;
    };
}
#endif // pMR_CLUSTERS_QPACEB_SENDMEMORYWINDOW_H
