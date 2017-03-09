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

pMR::Verbs::DeviceList::DeviceList()
{
    mDeviceList = ibv_get_device_list(&mDevices);
}

pMR::Verbs::DeviceList::~DeviceList()
{
    ibv_free_device_list(mDeviceList);
}

ibv_device *pMR::Verbs::DeviceList::operator[](int const index)
{
    return mDeviceList[index];
}

ibv_device const *pMR::Verbs::DeviceList::operator[](int const index) const
{
    return mDeviceList[index];
}

ibv_device **pMR::Verbs::DeviceList::begin()
{
    return &mDeviceList[0];
}

ibv_device *const *pMR::Verbs::DeviceList::begin() const
{
    return &mDeviceList[0];
}

ibv_device *const *pMR::Verbs::DeviceList::cbegin() const
{
    return &mDeviceList[0];
}

ibv_device **pMR::Verbs::DeviceList::end()
{
    return &mDeviceList[mDevices];
}

ibv_device *const *pMR::Verbs::DeviceList::end() const
{
    return &mDeviceList[mDevices];
}

ibv_device *const *pMR::Verbs::DeviceList::cend() const
{
    return &mDeviceList[mDevices];
}

int pMR::Verbs::DeviceList::size() const
{
    return {mDevices};
}

pMR::Verbs::Device::Device(
    std::shared_ptr<DeviceList> deviceList, int const device)
    : mDevice{device}, mDevices(deviceList)
{
}

ibv_device *pMR::Verbs::Device::get()
{
    return mDevices->operator[](mDevice);
}

ibv_device const *pMR::Verbs::Device::get() const
{
    return mDevices->operator[](mDevice);
}

std::string pMR::Verbs::Device::getName() const
{
    return mDevices->operator[](mDevice)->name;
}

std::string pMR::Verbs::Device::getPath() const
{
    return mDevices->operator[](mDevice)->dev_path;
}

ibv_node_type pMR::Verbs::Device::getNodeType() const
{
    return {mDevices->operator[](mDevice)->node_type};
}

ibv_transport_type pMR::Verbs::Device::getTransportType() const
{
    return {mDevices->operator[](mDevice)->transport_type};
}

pMR::Verbs::Devices::Devices() : mDevices(std::make_shared<DeviceList>())
{
}

pMR::Verbs::Device pMR::Verbs::Devices::operator[](int const index)
{
    return Device(mDevices, index);
}

pMR::Verbs::Device pMR::Verbs::Devices::operator[](int const index) const
{
    return Device(mDevices, index);
}

pMR::Verbs::Device pMR::Verbs::Devices::at(int const index)
{
    checkBoundaries(index);
    return operator[](index);
}

pMR::Verbs::Device pMR::Verbs::Devices::at(int const index) const
{
    checkBoundaries(index);
    return operator[](index);
}

int pMR::Verbs::Devices::size() const
{
    return mDevices->size();
}

void pMR::Verbs::Devices::checkBoundaries(int const index) const
{
    if(index < 0 || index >= size())
    {
        throw std::out_of_range("Desired device unavailable");
    }
}
