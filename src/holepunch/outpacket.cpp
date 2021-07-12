#include "holepunch/outpacket.hpp"

HolepunchOutPacket::HolepunchOutPacket(HolepunchPortId updatedPortId)
{
    this->Write<std::uint8_t>(HOLEPUNCH_PACKET_SIGNATURE);
    this->Write(std::uint8_t(HolepunchPacketType::Punch));
    this->Write(std::uint8_t(updatedPortId));
}
