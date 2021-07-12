#include "packets/in/room.hpp"

#include "packets/view.hpp"

InRoomPacket::InRoomPacket(PacketView& packet)
{
    this->Type = static_cast<InRoomPacketType>(packet.Read<std::uint8_t>());
}
