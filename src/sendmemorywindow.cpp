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

#include "sendmemorywindow.hpp"
#include "connection.hpp"

#ifdef pMR_PROVIDER_CMA
#include <cstdint>
#include "../../providers/cma/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
#include "../../providers/mpi/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
#include "../../providers/null/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
#include <cstdint>
#include "../../providers/ofi/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
#include <cstdint>
#include "../../providers/psm2/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
#include "../../providers/scif/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
#include "../../providers/self/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
#include <cstdint>
#include "../../providers/verbs/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_VERBS

pMR::SendMemoryWindow::SendMemoryWindow(
    Connection const &connection, void *buffer, size_type const sizeByte)
    : mBuffer(buffer), mSizeByte{sizeByte}, mProvider{connection.mProvider}
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::CMA:
        {
            mCMA = std::unique_ptr<CMA::SendMemoryWindow,
                CMA::SendMemoryWindowDeleter>(new CMA::SendMemoryWindow(
                connection.mCMA, buffer, {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::MPI:
        {
            mMPI = std::unique_ptr<MPI::SendMemoryWindow,
                MPI::SendMemoryWindowDeleter>(new MPI::SendMemoryWindow(
                connection.mMPI, buffer, {static_cast<int>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::Null:
        {
            mNull = std::unique_ptr<Null::SendMemoryWindow,
                Null::SendMemoryWindowDeleter>(new Null::SendMemoryWindow(
                connection.mNull, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::OFI:
        {
            mOFI = std::unique_ptr<OFI::SendMemoryWindow,
                OFI::SendMemoryWindowDeleter>(new OFI::SendMemoryWindow(
                connection.mOFI, buffer, {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::PSM2:
        {
            mPSM2 = std::unique_ptr<PSM2::SendMemoryWindow,
                PSM2::SendMemoryWindowDeleter>(
                new PSM2::SendMemoryWindow(connection.mPSM2, buffer,
                    {static_cast<std::uint32_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::SCIF:
        {
            mSCIF = std::unique_ptr<SCIF::SendMemoryWindow,
                SCIF::SendMemoryWindowDeleter>(
                new SCIF::SendMemoryWindow(connection.mSCIF, buffer,
                    {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::Self:
        {
            mSelf = std::unique_ptr<Self::SendMemoryWindow,
                Self::SendMemoryWindowDeleter>(
                new Self::SendMemoryWindow(connection.mSelf, buffer,
                    {static_cast<std::size_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::Verbs:
        {
            mVerbs = std::unique_ptr<Verbs::SendMemoryWindow,
                Verbs::SendMemoryWindowDeleter>(
                new Verbs::SendMemoryWindow(connection.mVerbs, buffer,
                    {static_cast<std::uint32_t>(sizeByte)}));
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

pMR::SendMemoryWindow::~SendMemoryWindow()
{
}

void pMR::SendMemoryWindow::init()
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::CMA:
        {
            mCMA->init();
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::MPI:
        {
            mMPI->init();
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::Null:
        {
            mNull->init();
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::OFI:
        {
            mOFI->init();
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::PSM2:
        {
            mPSM2->init();
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::SCIF:
        {
            mSCIF->init();
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::Self:
        {
            mSelf->init();
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::Verbs:
        {
            mVerbs->init();
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

void pMR::SendMemoryWindow::post()
{
    post(mSizeByte);
}

void pMR::SendMemoryWindow::post(size_type const sizeByte)
{
    if(sizeByte > mSizeByte)
    {
        throw std::length_error("pMR: Send message exceeds SendWindow.");
    }

    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::CMA:
        {
            mCMA->post({static_cast<std::size_t>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::MPI:
        {
            mMPI->post({static_cast<int>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::Null:
        {
            mNull->post({sizeByte});
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::OFI:
        {
            mOFI->post({static_cast<std::size_t>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::PSM2:
        {
            mPSM2->post({static_cast<std::uint32_t>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::SCIF:
        {
            mSCIF->post({static_cast<std::size_t>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::Self:
        {
            mSelf->post({static_cast<std::size_t>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::Verbs:
        {
            mVerbs->post({static_cast<std::uint32_t>(sizeByte)});
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

void pMR::SendMemoryWindow::wait()
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::CMA:
        {
            mCMA->wait();
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::MPI:
        {
            mMPI->wait();
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::Null:
        {
            mNull->wait();
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::OFI:
        {
            mOFI->wait();
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        case Provider::PSM2:
        {
            mPSM2->wait();
            break;
        }
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        case Provider::SCIF:
        {
            mSCIF->wait();
            break;
        }
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        case Provider::Self:
        {
            mSelf->wait();
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::Verbs:
        {
            mVerbs->wait();
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

void *pMR::SendMemoryWindow::data()
{
    return mBuffer;
}

void const *pMR::SendMemoryWindow::data() const
{
    return mBuffer;
}

pMR::size_type pMR::SendMemoryWindow::size() const
{
    return {mSizeByte};
}
