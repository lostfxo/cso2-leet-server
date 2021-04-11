#ifndef __IN_PACKET_ROOM_JOINREQ_H_
#define __IN_PACKET_ROOM_JOINREQ_H_

#include <cstdint>
#include <string>

class PacketView;

class InRoomPacketJoinRequest
{
public:
    InRoomPacketJoinRequest(PacketView& packet);

    // ordered for memory
    std::string Password;
    std::uint16_t Id;
};

#endif  // __IN_PACKET_ROOM_JOINREQ_H_
