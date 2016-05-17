#ifndef pMR_PROVIDERS_VERBS_DEVICE_H
#define pMR_PROVIDERS_VERBS_DEVICE_H

#include <memory>
#include <string>
#include <cstdint>
extern "C"
{
#include <infiniband/verbs.h>
}

namespace pMR { namespace verbs
{
    class DeviceList
    {
        public:
            DeviceList();
            DeviceList(const DeviceList&) = delete;
            DeviceList(DeviceList&&) = delete;
            DeviceList& operator=(const DeviceList&) = delete;
            DeviceList& operator=(DeviceList&&) = delete;
            ~DeviceList();
            ibv_device* operator[](std::size_t const);
            ibv_device const* operator[](std::size_t const) const;
            ibv_device** begin();
            ibv_device* const* begin() const;
            ibv_device* const* cbegin() const;
            ibv_device** end();
            ibv_device* const* end() const;
            ibv_device* const* cend() const;
            std::size_t size() const;
        private:
            int mDevices = 0;
            ibv_device **mDeviceList;
    };

    class Device
    {
        public:
            Device(std::shared_ptr<DeviceList>, std::size_t const device);
            ~Device() = default;
            ibv_device* get(); 
            ibv_device const* get() const;
            std::string getName() const;
            std::string getPath() const;
            ibv_node_type getNodeType() const;
            ibv_transport_type getTransportType() const; 
        private:
            std::size_t mDevice;
            std::shared_ptr<DeviceList> mDevices;
    };

    class Devices
    {
        public:
            Devices();
            ~Devices() = default;
            Device operator[](std::size_t const);
            Device operator[](std::size_t const) const;
            Device at(std::size_t const);
            Device at(std::size_t const) const;
            std::size_t size() const;
        private:
            std::shared_ptr<DeviceList> const mDevices;
            void checkBoundaries(std::size_t const) const;
    };
}}
#endif // pMR_PROVIDERS_VERBS_DEVICE_H
