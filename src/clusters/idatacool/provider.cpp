#include "provider.hpp"
#include "../../providers/null/memorywindow.hpp"
#include "../../providers/self/memorywindow.hpp"
#include "../../providers/loop/memorywindow.hpp"
#include "../../providers/cma/memorywindow.hpp"
#include "../../providers/verbs/memorywindow.hpp"

void pMR::null::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::null::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}

void pMR::self::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::self::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}

void pMR::loop::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::loop::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}

void pMR::cma::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::cma::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}

void pMR::verbs::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::verbs::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
