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
            SendMemoryWindow(SendMemoryWindow&&) = delete;
            SendMemoryWindow& operator=(const SendMemoryWindow&) = delete;
            SendMemoryWindow& operator=(SendMemoryWindow&&) = delete;
            ~SendMemoryWindow();
            void init();
            void post();
            void wait();
        private:
            Provider mProvider = Provider::null;
            std::unique_ptr<null::SendMemoryWindow> mNull;
            std::unique_ptr<self::SendMemoryWindow> mSelf;
            std::unique_ptr<loop::SendMemoryWindow> mLoop;
            std::unique_ptr<verbs::SendMemoryWindow> mVerbs;
    };
}
#endif // pMR_CLUSTERS_QPACEB_SENDMEMORYWINDOW_H
