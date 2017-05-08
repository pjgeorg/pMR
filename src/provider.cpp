//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "provider.hpp"

#ifdef pMR_PROVIDER_CMA
#include "../../providers/cma/recvmemorywindow.hpp"
#include "../../providers/cma/sendmemorywindow.hpp"

void pMR::CMA::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::CMA::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
#include "../../providers/mpi/recvmemorywindow.hpp"
#include "../../providers/mpi/sendmemorywindow.hpp"

void pMR::MPI::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::MPI::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
#include "../../providers/null/recvmemorywindow.hpp"
#include "../../providers/null/sendmemorywindow.hpp"

void pMR::Null::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::Null::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
#include "../../providers/ofi/recvmemorywindow.hpp"
#include "../../providers/ofi/sendmemorywindow.hpp"

void pMR::OFI::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::OFI::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
#include "../../providers/psm2/recvmemorywindow.hpp"
#include "../../providers/psm2/sendmemorywindow.hpp"

void pMR::PSM2::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::PSM2::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
#include "../../providers/scif/recvmemorywindow.hpp"
#include "../../providers/scif/sendmemorywindow.hpp"

void pMR::SCIF::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::SCIF::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
#include "../../providers/self/recvmemorywindow.hpp"
#include "../../providers/self/sendmemorywindow.hpp"

void pMR::Self::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::Self::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
#include "../../providers/verbs/recvmemorywindow.hpp"
#include "../../providers/verbs/sendmemorywindow.hpp"

void pMR::Verbs::SendMemoryWindowDeleter::operator()(SendMemoryWindow *p) const
{
    delete p;
}

void pMR::Verbs::RecvMemoryWindowDeleter::operator()(RecvMemoryWindow *p) const
{
    delete p;
}
#endif // pMR_PROVIDER_VERBS
