#include "packets/in/host.hpp"

#include "packets/view.hpp"

InHostPacket::InHostPacket(PacketView& packet)
{
    this->Type = InHostPacketType(packet.Read<std::uint8_t>());
}
