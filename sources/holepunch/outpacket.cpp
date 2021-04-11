#include "holepunch/outpacket.hpp"

#include "holepunch/shared.hpp"

HolepunchOutPacket::HolepunchOutPacket(std::uint16_t updatedPortId)
{
    this->Write<std::uint8_t>(HOLEPUNCH_PACKET_SIGNATURE);
    this->Write<std::uint16_t>(updatedPortId);
}
