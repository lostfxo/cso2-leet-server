#ifndef __IN_PACKET_ROOM_NEWREQ_H_
#define __IN_PACKET_ROOM_NEWREQ_H_

#include "room/shared.hpp"

class PacketView;

class InRoomPacketNewRequest
{
public:
    InRoomPacketNewRequest(PacketView& packet);

    NewRoomRequestOptions Options;
};

#endif  // __IN_PACKET_ROOM_NEWREQ_H_
