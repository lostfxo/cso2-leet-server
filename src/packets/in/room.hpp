#ifndef __IN_PACKET_ROOM_H_
#define __IN_PACKET_ROOM_H_

#include <cstdint>

class PacketView;

enum class InRoomPacketType : std::uint8_t
{
    NewRoomRequest = 0,
    JoinRoomRequest = 1,
    LeaveRoomRequest = 3,
    ToggleReadyRequest = 4,
    GameStartRequest = 5,
    UpdateSettings = 6,
    OnCloseResultWindow = 7,
    SetUserTeamRequest = 9,
    CountdownRequest = 19
};

class InRoomPacket
{
public:
    InRoomPacket(PacketView& packet);

    InRoomPacketType Type;
};

#endif  // __IN_PACKET_ROOM_H_
