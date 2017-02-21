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

#include "connection.hpp"
#include <array>
#include "../../arch/processor.hpp"
#include "../../backends/backend.hpp"
#include "message.hpp"
#include "mmap.hpp"
#include "rma.hpp"

pMR::scif::Connection::Connection(Target const &target)
{
    Endpoint passiveEndpoint;

    std::array<Address, 2> originAddress = {
        mActiveEndpoint.bind(), passiveEndpoint.bind()};

    passiveEndpoint.listen();

    decltype(originAddress) targetAddress;
    backend::exchange(target, originAddress, targetAddress);

    mActiveEndpoint.connect(std::get<1>(targetAddress), {false});
    mPassiveEndpoint =
        passiveEndpoint.accept(std::get<0>(targetAddress), {true});
    while(!mActiveEndpoint.connect(std::get<1>(targetAddress), {false}))
    {
        // Waiting for connection to be established
    }

    mTargetMR = std::unique_ptr<MemoryRegion>(new MemoryRegion(mActiveEndpoint,
        mTarget.data(), {sizeof(mTarget)}, SCIF_PROT_READ | SCIF_PROT_WRITE));
    mLocalNotifyMR = std::unique_ptr<MemoryRegion>(
        new MemoryRegion(mActiveEndpoint, mLocalNotify.data(),
            {sizeof(mLocalNotify)}, SCIF_PROT_READ | SCIF_PROT_WRITE));
    mRemoteNotifyMR = std::unique_ptr<MemoryRegion>(
        new MemoryRegion(mPassiveEndpoint, mRemoteNotify.data(),
            {sizeof(mRemoteNotify)}, SCIF_PROT_READ | SCIF_PROT_WRITE));

    std::get<0>(mTarget) = {static_cast<off_t>(-1)};

    std::array<off_t, 2> originOffsets = {
        mTargetMR->get(), mRemoteNotifyMR->get()};
    decltype(originOffsets) targetOffsets;
    send(mActiveEndpoint, originOffsets);
    recv(mPassiveEndpoint, targetOffsets);

    mTargetRemote = mmap<off_t>(mPassiveEndpoint, std::get<0>(targetOffsets),
        {sizeof(mTarget)}, SCIF_PROT_READ | SCIF_PROT_WRITE);
    mNotifyRemote = std::get<1>(targetOffsets);
}

pMR::scif::Connection::~Connection()
{
    munmap(mTargetRemote, {sizeof(mTarget)});
}

pMR::scif::Endpoint &pMR::scif::Connection::getActiveEndpoint()
{
    return mActiveEndpoint;
}

pMR::scif::Endpoint const &pMR::scif::Connection::getActiveEndpoint() const
{
    return mActiveEndpoint;
}

pMR::scif::PeerEndpoint &pMR::scif::Connection::getPassiveEndpoint()
{
    return mPassiveEndpoint;
}

pMR::scif::PeerEndpoint const &pMR::scif::Connection::getPassiveEndpoint() const
{
    return mPassiveEndpoint;
}

void pMR::scif::Connection::postAddress(off_t const localAddress)
{
    *mTargetRemote = {localAddress};
}

off_t pMR::scif::Connection::pollAddress()
{
    while(std::get<0>(mTarget) == static_cast<off_t>(-1))
    {
        CPURelax();
    }

    auto target = std::get<0>(mTarget);

    std::get<0>(mTarget) = static_cast<off_t>(-1);

    return {target};
}

void pMR::scif::Connection::writeData(
    MemoryRegion const &memoryRegion, std::size_t const sizeByte)
{
    if(sizeByte > 0)
    {
        write(mActiveEndpoint, memoryRegion, {pollAddress()}, {sizeByte});
    }
    else
    {
        pollAddress();
    }

    fenceSignal(mActiveEndpoint, mLocalNotifyMR->get(), 1, mNotifyRemote, 1);
}

void pMR::scif::Connection::pollLocalNotify()
{
    pollNotify(mLocalNotify);
}

void pMR::scif::Connection::pollRemoteNotify()
{
    pollNotify(mRemoteNotify);
}

void pMR::scif::Connection::pollNotify(
    std::array<std::uint64_t, cacheLineSize<std::uint64_t>()> &notify)
{
    while(std::get<0>(notify) == 0)
    {
        CPURelax();
    }

    std::get<0>(notify) = 0;
}
