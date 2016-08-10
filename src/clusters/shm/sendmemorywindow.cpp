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
#include "../../providers/null/memorywindow.hpp"
#include "../../providers/self/memorywindow.hpp"
#include "../../providers/loop/memorywindow.hpp"
#include "../../providers/cma/memorywindow.hpp"

pMR::SendMemoryWindow::SendMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mBuffer(buffer),
        mSizeByte(sizeByte),
        mProvider(connection.mProvider)
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull = std::unique_ptr<null::SendMemoryWindow,
                    null::SendMemoryWindowDeleter>(new null::SendMemoryWindow(
                                connection.mNull, buffer, sizeByte));
            break;
        }
        case Provider::self:
        {
            mSelf = std::unique_ptr<self::SendMemoryWindow,
                    self::SendMemoryWindowDeleter>(new self::SendMemoryWindow(
                                connection.mSelf, buffer, sizeByte));
            break;
        }
        case Provider::loop:
        {
            mLoop = std::unique_ptr<loop::SendMemoryWindow,
                    loop::SendMemoryWindowDeleter>(new loop::SendMemoryWindow(
                                connection.mLoop, buffer, sizeByte));
            break;
        }
        case Provider::cma:
        {
            mCMA = std::unique_ptr<cma::SendMemoryWindow,
                    cma::SendMemoryWindowDeleter>(new cma::SendMemoryWindow(
                                connection.mCMA, buffer, sizeByte));
            break;
        }
    }
}

pMR::SendMemoryWindow::~SendMemoryWindow() { }

void pMR::SendMemoryWindow::init()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->init();
            break;
        }
        case Provider::self:
        {
            mSelf->init();
            break;
        }
        case Provider::loop:
        {
            mLoop->init();
            break;
        }
        case Provider::cma:
        {
            mCMA->init();
            break;
        }
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
        throw std::length_error("pMR: Send message exceed SendWindow.");
    }

    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->post(sizeByte);
            break;
        }
        case Provider::self:
        {
            mSelf->post(sizeByte);
            break;
        }
        case Provider::loop:
        {
            mLoop->post(sizeByte);
            break;
        }
        case Provider::cma:
        {
            mCMA->post(sizeByte);
            break;
        }
    }
}

void pMR::SendMemoryWindow::wait()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->wait();
            break;
        }
        case Provider::self:
        {
            mSelf->wait();
            break;
        }
        case Provider::loop:
        {
            mLoop->wait();
            break;
        }
        case Provider::cma:
        {
            mCMA->wait();
            break;
        }
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
