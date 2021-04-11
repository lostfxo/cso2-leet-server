#ifndef __IN_PACKET_ROOM_COUNTDOWN_H_
#define __IN_PACKET_ROOM_COUNTDOWN_H_

#include <cstdint>

class PacketView;

class InRoomPacketCountdownRequest
{
public:
    InRoomPacketCountdownRequest(PacketView& packet);

    bool CountingDown;
    std::uint8_t CountNum;
};

#endif  // __IN_PACKET_ROOM_COUNTDOWN_H_
