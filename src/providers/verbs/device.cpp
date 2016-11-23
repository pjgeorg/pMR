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

#include "device.hpp"
#include <stdexcept>

pMR::verbs::DeviceList::DeviceList()
{
    mDeviceList = ibv_get_device_list(&mDevices);
}

pMR::verbs::DeviceList::~DeviceList()
{
    ibv_free_device_list(mDeviceList);
}

ibv_device* pMR::verbs::DeviceList::operator[](int const index)
{
    return mDeviceList[index];
}

ibv_device const* pMR::verbs::DeviceList::operator[](int const index)
    const
{
    return mDeviceList[index];
}

ibv_device** pMR::verbs::DeviceList::begin()
{
    return &mDeviceList[0];
}

ibv_device* const* pMR::verbs::DeviceList::begin() const
{
    return &mDeviceList[0];
}

ibv_device* const* pMR::verbs::DeviceList::cbegin() const
{
    return &mDeviceList[0];
}

ibv_device** pMR::verbs::DeviceList::end()
{
    return &mDeviceList[mDevices];
}

ibv_device* const* pMR::verbs::DeviceList::end() const
{
    return &mDeviceList[mDevices];
}

ibv_device* const* pMR::verbs::DeviceList::cend() const
{
    return &mDeviceList[mDevices];
}

int pMR::verbs::DeviceList::size() const
{
    return {mDevices};
}

pMR::verbs::Device::Device(std::shared_ptr<DeviceList> deviceList,
        int const device)
    :   mDevice{device},
        mDevices(deviceList) { }

ibv_device* pMR::verbs::Device::get()
{
    return mDevices->operator[](mDevice);
}

ibv_device const* pMR::verbs::Device::get() const
{
    return mDevices->operator[](mDevice);
}

std::string pMR::verbs::Device::getName() const
{
    return mDevices->operator[](mDevice)->name;
}

std::string pMR::verbs::Device::getPath() const
{
    return mDevices->operator[](mDevice)->dev_path;
}

ibv_node_type pMR::verbs::Device::getNodeType() const
{
    return {mDevices->operator[](mDevice)->node_type};
}

ibv_transport_type pMR::verbs::Device::getTransportType() const
{
    return {mDevices->operator[](mDevice)->transport_type};
}

pMR::verbs::Devices::Devices()
    :   mDevices(std::make_shared<DeviceList>()) { }

pMR::verbs::Device pMR::verbs::Devices::operator[](int const index)
{
    return Device(mDevices, index);
}

pMR::verbs::Device pMR::verbs::Devices::operator[](int const index)
    const
{
    return Device(mDevices, index);
}

pMR::verbs::Device pMR::verbs::Devices::at(int const index)
{
    checkBoundaries(index);
    return operator[](index);
}

pMR::verbs::Device pMR::verbs::Devices::at(int const index) const
{
    checkBoundaries(index);
    return operator[](index);
}

int pMR::verbs::Devices::size() const
{
    return mDevices->size();
}

void pMR::verbs::Devices::checkBoundaries(int const index) const
{
    if(index < 0 || index >= size())
    {
        throw std::out_of_range("Desired device unavailable");
    }
}
