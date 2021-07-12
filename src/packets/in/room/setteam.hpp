#ifndef __IN_PACKET_ROOM_SETTEAM_H_
#define __IN_PACKET_ROOM_SETTEAM_H_

#include "cso2/definitions.hpp"

class PacketView;

class InRoomPacketSetTeamRequest
{
public:
    InRoomPacketSetTeamRequest(PacketView& packet);

    cso2::TeamNum Team;
};

#endif  // __IN_PACKET_ROOM_SETTEAM_H_
