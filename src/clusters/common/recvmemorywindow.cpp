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

#include "recvmemorywindow.hpp"
#include "connection.hpp"

#ifdef pMR_PROVIDER_CMA
#include <cstdint>
#include "../../providers/cma/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
#include "../../providers/mpi/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
#include "../../providers/null/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
#include <cstdint>
#include "../../providers/ofi/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
#include <cstdint>
#include "../../providers/psm2/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
#include <cstdint>
#include "../../providers/scif/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
#include <cstdint>
#include "../../providers/self/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
#include <cstdint>
#include "../../providers/verbs/recvmemorywindow.hpp"
#endif // pMR_PROVIDER_VERBS

pMR::RecvMemoryWindow::RecvMemoryWindow(
    Connection const &connection, void *buffer, size_type const sizeByte)
    : mBuffer(buffer), mSizeByte{sizeByte}, mProvider{connection.mProvider}
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::cma:
        {
            mCMA = std::unique_ptr<cma::RecvMemoryWindow,
                cma::RecvMemoryWindowDeleter>(new cma::RecvMemoryWindow(
                connection.mCMA, buffer, {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::mpi:
        {
            mMPI = std::unique_ptr<mpi::RecvMemoryWindow,
                mpi::RecvMemoryWindowDeleter>(new mpi::RecvMemoryWindow(
                connection.mMPI, buffer, {static_cast<int>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::null:
        {
            mNull = std::unique_ptr<null::RecvMemoryWindow,
                null::RecvMemoryWindowDeleter>(new null::RecvMemoryWindow(
                connection.mNull, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::ofi:
        {
            mOFI = std::unique_ptr<ofi::RecvMemoryWindow,
                ofi::RecvMemoryWindowDeleter>(new ofi::RecvMemoryWindow(
                connection.mOFI, buffer, {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::psm2:
        {
            mPSM2 = std::unique_ptr<psm2::RecvMemoryWindow,
                psm2::RecvMemoryWindowDeleter>(
                new psm2::RecvMemoryWindow(connection.mPSM2, buffer,
                    {static_cast<std::uint32_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::scif:
        {
            mSCIF = std::unique_ptr<scif::RecvMemoryWindow,
                scif::RecvMemoryWindowDeleter>(
                new scif::RecvMemoryWindow(connection.mSCIF, buffer,
                    {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::self:
        {
            mSelf = std::unique_ptr<self::RecvMemoryWindow,
                self::RecvMemoryWindowDeleter>(
                new self::RecvMemoryWindow(connection.mSelf, buffer,
                    {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::verbs:
        {
            mVerbs = std::unique_ptr<verbs::RecvMemoryWindow,
                verbs::RecvMemoryWindowDeleter>(
                new verbs::RecvMemoryWindow(connection.mVerbs, buffer,
                    {static_cast<std::uint32_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

pMR::RecvMemoryWindow::~RecvMemoryWindow()
{
}

void pMR::RecvMemoryWindow::init()
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::cma:
        {
            mCMA->init();
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::mpi:
        {
            mMPI->init();
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::null:
        {
            mNull->init();
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::ofi:
        {
            mOFI->init();
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::psm2:
        {
            mPSM2->init();
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::scif:
        {
            mSCIF->init();
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::self:
        {
            mSelf->init();
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::verbs:
        {
            mVerbs->init();
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

void pMR::RecvMemoryWindow::post()
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::cma:
        {
            mCMA->post();
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::mpi:
        {
            mMPI->post();
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::null:
        {
            mNull->post();
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::ofi:
        {
            mOFI->post();
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::psm2:
        {
            mPSM2->post();
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::scif:
        {
            mSCIF->post();
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::self:
        {
            mSelf->post();
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::verbs:
        {
            mVerbs->post();
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

void pMR::RecvMemoryWindow::wait()
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::cma:
        {
            mCMA->wait();
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::mpi:
        {
            mMPI->wait();
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::null:
        {
            mNull->wait();
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::ofi:
        {
            mOFI->wait();
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::psm2:
        {
            mPSM2->wait();
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::scif:
        {
            mSCIF->wait();
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::self:
        {
            mSelf->wait();
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::verbs:
        {
            mVerbs->wait();
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

void *pMR::RecvMemoryWindow::data()
{
    return mBuffer;
}

void const *pMR::RecvMemoryWindow::data() const
{
    return mBuffer;
}

pMR::size_type pMR::RecvMemoryWindow::size() const
{
    return {mSizeByte};
}
