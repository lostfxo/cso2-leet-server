#include "packets/in/requestroomlist.hpp"

#include "packets/view.hpp"

InRequestRoomListPacket::InRequestRoomListPacket(PacketView& packet)
{
    this->ChannelGroupIndex = packet.Read<std::uint8_t>();
    this->ChannelIndex = packet.Read<std::uint8_t>();
}
