#ifndef __OUT_PKT_ROOMLIST_
#define __OUT_PKT_ROOMLIST_

#include "packets/builder.hpp"
#include "room/shared.hpp"

class OutRoomListPacket
{
public:
    static PacketBuilder SendRoomList(const RoomList& rooms);
};

#endif  // __OUT_PKT_ROOMLIST_
