#include "topology.hpp"
#include <fstream>
#include <iterator>
#include <string>
#include <stdexcept>
#include "support.hpp"
#include "../../misc/singleton.hpp"

int pMR::numa::getNode(std::string const devicePath)
{
    Singleton<Support>::Instance();

    std::ifstream deviceFile(devicePath + "/device/numa_node", std::ios::in);
    if(!deviceFile)
    {
        throw std::runtime_error("pMR: Unable to open device file.");
    }

    return *(std::istream_iterator<int>(deviceFile));
}
