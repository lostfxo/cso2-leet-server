#include "packets/in/room/setteam.hpp"

#include "packets/view.hpp"

InRoomPacketSetTeamRequest::InRoomPacketSetTeamRequest(PacketView& packet)
{
    this->Team = cso2::TeamNum(packet.Read<std::uint8_t>());
}
