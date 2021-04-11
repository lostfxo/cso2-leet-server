#ifndef __IN_PACKET_HOST_H_
#define __IN_PACKET_HOST_H_

#include <cstdint>

enum class InHostPacketType : std::uint8_t
{
    ChangeTeam = 11,

    SetInventory =
        101,  // there are 2 or 3 other host packet types that send this
    SetLoadout = 107,
    SetBuyMenu = 111,

    // events
    OnPlayerKilled = 3,
    OnPlayerScored = 7,
    OnRoundEnd = 10,
    OnPlayerBuyItem = 14,
    OnPlayerSpawn = 20,
    OnGameEnd = 21,
    OnPlayerItemUse = 105,
};

class PacketView;

class InHostPacket
{
public:
    InHostPacket(PacketView& packet);

    InHostPacketType Type;
};

#endif  // __IN_PACKET_HOST_H_
