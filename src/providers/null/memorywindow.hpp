#ifndef pMR_PROVIDERS_NULL_MEMORYWINDOW_H
#define pMR_PROVIDERS_NULL_MEMORYWINDOW_H

#include <memory>
#include <cstdint>

namespace pMR { namespace null
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
    };
}}
#endif // pMR_PROVIDERS_NULL_MEMORYWINDOW_H
