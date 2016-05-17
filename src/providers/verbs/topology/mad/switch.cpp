#include "switch.hpp"
#include "../../../../misc/random.hpp"

pMR::verbs::mad::SwitchLID::SwitchLID(Context &context,
        std::uint8_t const portNumber)
    :   MAD(context, portNumber)
{
    auto transactionID = getRandomNumber<std::uint32_t>();
    // SA Formate (15.2.1.1) (0-55)
    // Base Version 1Byte
    std::get<0>(mSendMAD) = 0x01;
    // Management Class 1Byte
    std::get<1>(mSendMAD) = 0x03;
    // Class Version 1Byte
    std::get<2>(mSendMAD) = 0x02; 
    // Method 1Byte
    std::get<3>(mSendMAD) = 0x01;
    // Status 2Byte
    // Class specific 2Byte
    // Transaction ID 8Byte
    std::get<12>(mSendMAD) = (transactionID & 0xFF000000) >> 24;
    std::get<13>(mSendMAD) = (transactionID & 0x00FF0000) >> 16;
    std::get<14>(mSendMAD) = (transactionID & 0x0000FF00) >> 8;
    std::get<15>(mSendMAD) = (transactionID & 0x000000FF);
    // Attribute ID 2Byte
    std::get<16>(mSendMAD) = 0x00;
    std::get<17>(mSendMAD) = 0x20;
    // Reserved 144 2Byte
    // Attribute Modifier 4Byte
    // RMPP 4Byte
    // DATA1 4Byte
    // DATA2 4Byte
    // SMKey 8Byte
    // Attribute Offset 2Byte
    // Reserved 368 2Byte
    // Component Mask 8Byte
    std::get<55>(mSendMAD) = 0x03;

    // LinkRecord (15.2.5.13) (56-63)
    // Source LID 2Byte
    std::get<56>(mSendMAD) = (mPortAttributes.getLID() & 0xFF00) >> 8;
    std::get<57>(mSendMAD) = (mPortAttributes.getLID() & 0x00FF);
    // Source Port 1Byte
    std::get<58>(mSendMAD) = portNumber;
    // Destination Port 1Byte
    // Destination LID 2Byte
    // Reserved 48 2Byte
}

bool pMR::verbs::mad::SwitchLID::validate()
{
    std::uint32_t status = std::get<VerbsGRHSize+4>(mRecvMAD) << 24 |
        std::get<VerbsGRHSize+5>(mRecvMAD) << 16 |
        std::get<VerbsGRHSize+6>(mRecvMAD) << 8 |
        std::get<VerbsGRHSize+75>(mRecvMAD);
    if(status)
    {
        return false;
    }

    std::uint8_t method = std::get<VerbsGRHSize+3>(mRecvMAD) & 0x0F;
    if(method != 0x01)
    {
        return false;
    }

    std::uint16_t offset = std::get<VerbsGRHSize+44>(mRecvMAD) << 8 |
        std::get<VerbsGRHSize+45>(mRecvMAD);
    if(!offset)
    {
        return false;
    }

    if(!std::equal(mRecvMAD.begin()+VerbsGRHSize+12,
                mRecvMAD.begin()+VerbsGRHSize+12+4, mSendMAD.begin()+12))
    {
        return false;
    }
    return true;
}

int pMR::verbs::mad::SwitchLID::getSwitchLID()
{
    do
    {
        query();
    }
    while(!validate());
    return (std::get<VerbsGRHSize+60>(mRecvMAD) |
            std::get<VerbsGRHSize+61>(mRecvMAD));
}
