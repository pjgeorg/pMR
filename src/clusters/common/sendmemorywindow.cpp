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
#   include "../../providers/cma/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
#   include "../../providers/mpi/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
#   include "../../providers/null/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
#   include "../../providers/ofi/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_SELF
#   include "../../providers/self/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
#   include "../../providers/verbs/sendmemorywindow.hpp"
#endif // pMR_PROVIDER_VERBS

pMR::SendMemoryWindow::SendMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mBuffer(buffer),
        mSizeByte{sizeByte},
        mProvider{connection.mProvider}
{
    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::cma:
        {
            mCMA = std::unique_ptr<cma::SendMemoryWindow,
                    cma::SendMemoryWindowDeleter>(new cma::SendMemoryWindow(
                                connection.mCMA, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::mpi:
        {
            mMPI = std::unique_ptr<mpi::SendMemoryWindow,
                    mpi::SendMemoryWindowDeleter>(new mpi::SendMemoryWindow(
                                connection.mMPI, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::null:
        {
            mNull = std::unique_ptr<null::SendMemoryWindow,
                    null::SendMemoryWindowDeleter>(new null::SendMemoryWindow(
                                connection.mNull, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::ofi:
        {
            mOFI = std::unique_ptr<ofi::SendMemoryWindow,
                    ofi::SendMemoryWindowDeleter>(new ofi::SendMemoryWindow(
                                connection.mOFI, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_SELF
        case Provider::self:
        {
            mSelf = std::unique_ptr<self::SendMemoryWindow,
                    self::SendMemoryWindowDeleter>(new self::SendMemoryWindow(
                                connection.mSelf, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::verbs:
        {
            mVerbs = std::unique_ptr<verbs::SendMemoryWindow,
                    verbs::SendMemoryWindowDeleter>(new verbs::SendMemoryWindow(
                                connection.mVerbs, buffer, {sizeByte}));
            break;
        }
#endif // pMR_PROVIDER_VERBS
    }
}

pMR::SendMemoryWindow::~SendMemoryWindow() { }

void pMR::SendMemoryWindow::init()
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

void pMR::SendMemoryWindow::post()
{
    post(mSizeByte);
}

void pMR::SendMemoryWindow::post(std::uint32_t const sizeByte)
{
    if(sizeByte > mSizeByte)
    {
        throw std::length_error("pMR: Send message exceeds SendWindow.");
    }

    switch(mProvider)
    {
#ifdef pMR_PROVIDER_CMA
        case Provider::cma:
        {
            mCMA->post({sizeByte});
            break;
        }
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        case Provider::mpi:
        {
            mMPI->post({sizeByte});
            break;
        }
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        case Provider::null:
        {
            mNull->post({sizeByte});
            break;
        }
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        case Provider::ofi:
        {
            mOFI->post({sizeByte});
            break;
        }
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_SELF
        case Provider::self:
        {
            mSelf->post({sizeByte});
            break;
        }
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        case Provider::verbs:
        {
            mVerbs->post({sizeByte});
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

void* pMR::SendMemoryWindow::data()
{
    return mBuffer;
}

void const* pMR::SendMemoryWindow::data() const
{
    return mBuffer;
}

std::uint32_t pMR::SendMemoryWindow::size() const
{
    return mSizeByte;
}
