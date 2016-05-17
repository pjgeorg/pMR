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

ibv_device* pMR::verbs::DeviceList::operator[](std::size_t const index)
{
    return mDeviceList[index];
}

ibv_device const* pMR::verbs::DeviceList::operator[](std::size_t const index)
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

std::size_t pMR::verbs::DeviceList::size() const
{
    return mDevices;
}

pMR::verbs::Device::Device(std::shared_ptr<DeviceList> deviceList,
        std::size_t const device)
    :   mDevice(device),
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
    return mDevices->operator[](mDevice)->node_type;
}

ibv_transport_type pMR::verbs::Device::getTransportType() const
{
    return mDevices->operator[](mDevice)->transport_type;
}

pMR::verbs::Devices::Devices()
    :   mDevices(std::make_shared<DeviceList>()) { }

pMR::verbs::Device pMR::verbs::Devices::operator[](std::size_t const index)
{
    return Device(mDevices, index);
}

pMR::verbs::Device pMR::verbs::Devices::operator[](std::size_t const index)
    const
{
    return Device(mDevices, index);
}

pMR::verbs::Device pMR::verbs::Devices::at(std::size_t const index)
{
    checkBoundaries(index);
    return operator[](index);
}

pMR::verbs::Device pMR::verbs::Devices::at(std::size_t const index) const
{
    checkBoundaries(index);
    return operator[](index);
}

std::size_t pMR::verbs::Devices::size() const
{
    return mDevices->size();
}

void pMR::verbs::Devices::checkBoundaries(std::size_t const index) const
{
    if(index < 0 || index >= size())
    {
        throw std::out_of_range("Desired device unavailable");
    }
}
