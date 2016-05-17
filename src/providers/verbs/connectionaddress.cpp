#include "connectionaddress.hpp"
#include <array>
#include <stdexcept>
#include "portattributes.hpp"
#include "verbs.hpp"
#include "../../backends/backend.hpp"

pMR::verbs::ConnectionAddress::ConnectionAddress(Context &context,
        QueuePair const &queuePair, std::uint8_t const portNumber)
    :   mQPN(queuePair.getQPN()), mGID(context, portNumber)
{
    PortAttributes portAttributes(context, portNumber);
    mLID = portAttributes.getLID();
    mMTU = portAttributes.getMTU();
}

pMR::verbs::ConnectionAddress::ConnectionAddress(
        ConnectionAddress const &connectionAddress,
        QueuePair const &queuePair)
    :   mQPN(queuePair.getQPN()),
        mGID(connectionAddress.getGID()),
        mLID(connectionAddress.getLID()),
        mMTU(connectionAddress.getMTU()) { }

void pMR::verbs::ConnectionAddress::setQPN(std::uint32_t const QPN)
{
    mQPN = QPN;
}

void pMR::verbs::ConnectionAddress::setLID(std::uint16_t const LID)
{
    mLID = LID;
}

void pMR::verbs::ConnectionAddress::setGUID(std::uint64_t const GUID)
{
    mGID.setGUID(GUID);
}

void pMR::verbs::ConnectionAddress::setSubnetPrefix(
        std::uint64_t const subnetPrefix)
{
    mGID.setSubnetPrefix(subnetPrefix);
}

void pMR::verbs::ConnectionAddress::setMTU(ibv_mtu const MTU)
{
    mMTU = MTU;
}

std::uint32_t pMR::verbs::ConnectionAddress::getQPN() const
{
    return mQPN;
}

std::uint16_t pMR::verbs::ConnectionAddress::getLID() const
{
    return mLID;
}

ibv_gid pMR::verbs::ConnectionAddress::getGID() const
{
    return mGID.get();
}

std::uint64_t pMR::verbs::ConnectionAddress::getGUID() const
{
    return mGID.getGUID();
}

std::uint64_t pMR::verbs::ConnectionAddress::getSubnetPrefix() const
{
    return mGID.getSubnetPrefix();
}

ibv_mtu pMR::verbs::ConnectionAddress::getMTU() const
{
    return mMTU;
}

void pMR::verbs::exchangeConnectionAddress(pMR::Target const &target,
        ConnectionAddress const &originAddress,
        ConnectionAddress &targetAddress)
{
    std::array<std::uint64_t, 5> sendBuffer = {
        originAddress.getQPN(),
        originAddress.getLID(),
        originAddress.getSubnetPrefix(),
        originAddress.getGUID(),
        originAddress.getMTU(),
    };
    decltype(sendBuffer) recvBuffer;

    pMR::backend::exchange(target, sendBuffer, recvBuffer);

    targetAddress.setQPN(std::get<0>(recvBuffer));
    targetAddress.setLID(std::get<1>(recvBuffer));
    targetAddress.setSubnetPrefix(std::get<2>(recvBuffer));
    targetAddress.setGUID(std::get<3>(recvBuffer));
    targetAddress.setMTU(static_cast<ibv_mtu>(std::get<4>(recvBuffer)));

    if(originAddress.getMTU() != targetAddress.getMTU())
    {
        throw std::runtime_error("pMR: Connection MTU mismatch.");
    }
}
