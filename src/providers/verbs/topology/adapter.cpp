#include "../topology.hpp"
#include <stdexcept>

std::vector<pMR::verbs::Device>::size_type pMR::verbs::getAdapter(
        Devices const &devices, std::vector<Device> &deviceList,
        ibv_node_type const nodeType, ibv_transport_type const transportType)
{
    for(std::vector<Device>::size_type i = 0; i != devices.size(); ++i)
    {
        if(devices[i].getNodeType() == nodeType
                && devices[i].getTransportType() == transportType)
        {
            deviceList.push_back(devices[i]);
        }
    }
    return {deviceList.size()};
}

pMR::verbs::Device pMR::verbs::getIBAdapter(Devices const &devices,
        int const deviceNumber)
{
    std::vector<Device> deviceList;

    if(!getAdapter(devices, deviceList, IBV_NODE_CA, IBV_TRANSPORT_IB))
    {
        throw std::runtime_error("pMR: No Infiniband adapter found.");
    }
    return deviceList.at(deviceNumber);
}
