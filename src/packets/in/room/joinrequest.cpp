#include "packets/in/room/joinrequest.hpp"

#include "packets/view.hpp"

InRoomPacketJoinRequest::InRoomPacketJoinRequest(PacketView& packet)
{
    this->Id = packet.Read<std::uint16_t>();
    this->Password = packet.ReadString();
}
