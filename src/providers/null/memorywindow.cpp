#include "memorywindow.hpp"

pMR::null::SendMemoryWindow::SendMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte) { }

pMR::null::RecvMemoryWindow::RecvMemoryWindow(
        std::shared_ptr<Connection> const connection,
        void *buffer, std::uint32_t const sizeByte) { }

void pMR::null::SendMemoryWindow::init() { }

void pMR::null::SendMemoryWindow::post() { }

void pMR::null::SendMemoryWindow::wait() { }

void pMR::null::RecvMemoryWindow::init() { }

void pMR::null::RecvMemoryWindow::post() { }

void pMR::null::RecvMemoryWindow::wait() { }
