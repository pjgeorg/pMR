#ifndef pMR_PROVIDERS_LOOP_MEMORYWINDOW_H
#define pMR_PROVIDERS_LOOP_MEMORYWINDOW_H

#include <memory>
#include <cstdint>

namespace pMR { namespace loop
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
            void *mBuffer = nullptr;
            std::uint32_t const mSizeByte;
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
            void *mBuffer = nullptr;
            std::uint32_t const mSizeByte;
    };
}}
#endif // pMR_PROVIDERS_LOOP_MEMORYWINDOW_H
