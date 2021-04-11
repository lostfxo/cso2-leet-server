#ifndef __IN_PACKET_ROOM_UPDATESETTINGS_H_
#define __IN_PACKET_ROOM_UPDATESETTINGS_H_

#include <cstdint>
#include <string>

#include "room/settings.hpp"

class PacketView;

class InRoomPacketUpdateSettings
{
public:
    InRoomPacketUpdateSettings(PacketView& packet);

    std::uint64_t Flags;
    RoomSettings Settings;
};

#endif  // __IN_PACKET_ROOM_UPDATESETTINGS_H_
