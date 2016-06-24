#ifndef pMR_PROVIDERS_CMA_MEMORYWINDOW_H
#define pMR_PROVIDERS_CMA_MEMORYWINDOW_H

#include <memory>
#include <cstdint>
extern "C"
{
#include <sys/uio.h>
}

namespace pMR { namespace cma
{
    class Connection;

    class SendMemoryWindow
    {
        public:
            SendMemoryWindow(std::shared_ptr<Connection> const,
                    void *buffer, std::uint32_t const sizeByte);
            SendMemoryWindow(const SendMemoryWindow&) = delete;
            SendMemoryWindow(SendMemoryWindow&&) = delete;
            SendMemoryWindow& operator=(const SendMemoryWindow&) = delete;
            SendMemoryWindow& operator=(SendMemoryWindow&&) = delete;
            ~SendMemoryWindow() = default;
            void init();
            void post(std::uint32_t const sizeByte);
            void wait();
        private:
            std::shared_ptr<Connection> const mConnection;
            iovec mBuffer;
    };

    class RecvMemoryWindow
    {
        public:
            RecvMemoryWindow(std::shared_ptr<Connection> const,
                    void *buffer, std::uint32_t const sizeByte);
            RecvMemoryWindow(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow(RecvMemoryWindow&&) = delete;
            RecvMemoryWindow& operator=(const RecvMemoryWindow&) = delete;
            RecvMemoryWindow& operator=(RecvMemoryWindow&&) = delete;
            ~RecvMemoryWindow() = default;
            void init();
            void post();
            void wait();
        private:
            std::shared_ptr<Connection> const mConnection;
            iovec mBuffer;
    };
}}
#endif // pMR_PROVIDERS_CMA_MEMORYWINDOW_H
