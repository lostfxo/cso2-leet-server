#ifndef __OUT_PKT_ROOM_
#define __OUT_PKT_ROOM_

#include "packets/builder.hpp"
#include "room/shared.hpp"

enum class OutRoomPacketType : std::uint8_t
{
    CreateAndJoin = 0,
    PlayerJoin = 1,
    PlayerLeave = 2,
    SetPlayerReady = 3,
    UpdateSettings = 4,
    SetHost = 5,
    SetGameResult = 6,
    SetUserTeam = 7,
    Countdown = 14
};

class OutRoomPacket
{
public:
    static PacketBuilder CreateAndJoin(const Room& room);
    static PacketBuilder PlayerJoin(const SlotPtr slot);
    static PacketBuilder PlayerLeave(std::uint32_t userId);
    static PacketBuilder SetPlayerReady(std::uint32_t userId,
                                        cso2::SlotStatus status);
    static PacketBuilder UpdateSettings(const RoomSettings& settings);
    static PacketBuilder UpdateStatusSettings(RoomStatus status, bool ingame);
    static PacketBuilder SetHost(std::uint32_t userId);
    static PacketBuilder SetGameResult();
    static PacketBuilder SetUserTeam(std::uint32_t userId, cso2::TeamNum team);
    static PacketBuilder ProgressCountdown(std::uint8_t countdown);
    static PacketBuilder StopCountdown();
};

#endif  // __OUT_PKT_ROOM_
