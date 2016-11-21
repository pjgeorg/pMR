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
#include "../../providers/null/recvmemorywindow.hpp"
#include "../../providers/self/recvmemorywindow.hpp"
#include "../../providers/verbs/recvmemorywindow.hpp"

pMR::RecvMemoryWindow::RecvMemoryWindow(Connection const &connection,
        void *buffer, std::uint32_t const sizeByte)
    :   mBuffer(buffer),
        mSizeByte(sizeByte),
        mProvider(connection.mProvider)
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull = std::unique_ptr<null::RecvMemoryWindow,
                    null::RecvMemoryWindowDeleter>(new null::RecvMemoryWindow(
                                connection.mNull, buffer, sizeByte));
            break;
        }
        case Provider::self:
        {
            mSelf = std::unique_ptr<self::RecvMemoryWindow,
                    self::RecvMemoryWindowDeleter>(new self::RecvMemoryWindow(
                                connection.mSelf, buffer, sizeByte));
            break;
        }
        case Provider::verbs:
        {
            mVerbs = std::unique_ptr<verbs::RecvMemoryWindow,
                    verbs::RecvMemoryWindowDeleter>(new verbs::RecvMemoryWindow(
                                connection.mVerbs, buffer, sizeByte));
            break;
        }
    }
}

pMR::RecvMemoryWindow::~RecvMemoryWindow() { }

void pMR::RecvMemoryWindow::init()
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
        case Provider::verbs:
        {
            mVerbs->init();
            break;
        }
    }
}

void pMR::RecvMemoryWindow::post()
{
    switch(mProvider)
    {
        case Provider::null:
        {
            mNull->post();
            break;
        }
        case Provider::self:
        {
            mSelf->post();
            break;
        }
        case Provider::verbs:
        {
            mVerbs->post();
            break;
        }
    }
}

void pMR::RecvMemoryWindow::wait()
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
        case Provider::verbs:
        {
            mVerbs->wait();
            break;
        }
    }
}

void* pMR::RecvMemoryWindow::data()
{
    return mBuffer;
}

void const* pMR::RecvMemoryWindow::data() const
{
    return mBuffer;
}

std::uint32_t pMR::RecvMemoryWindow::size() const
{
    return mSizeByte;
}
