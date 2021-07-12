#ifndef __IN_PACKET_REQUESTROOMLIST_H_
#define __IN_PACKET_REQUESTROOMLIST_H_

#include <cstdint>

class PacketView;

class InRequestRoomListPacket
{
public:
    InRequestRoomListPacket(PacketView& packet);

    std::uint8_t ChannelGroupIndex;
    std::uint8_t ChannelIndex;
};

#endif  // __IN_PACKET_REQUESTROOMLIST_H_
