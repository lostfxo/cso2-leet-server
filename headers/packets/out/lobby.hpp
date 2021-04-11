#ifndef __OUT_PKT_LOBBY_
#define __OUT_PKT_LOBBY_

#include "packets/builder.hpp"

class OutLobbyPacket
{
public:
    static PacketBuilder JoinRoomList();
};

#endif  // __OUT_PKT_LOBBY_
