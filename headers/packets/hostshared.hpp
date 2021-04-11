#ifndef __PKT_HOSTSHARED_H_
#define __PKT_HOSTSHARED_H_

#include <cstdint>

enum class HostPacketType : std::uint8_t
{
    GameStart = 0,  // when a host starts a new game
    HostJoin = 1,   // when someone joins some host's game
    HostStop = 3,
    Ingame_PlayerDeath = 3,
    LeaveResultWindow = 4,
    Ingame_AddPoint = 7,
    Ingame_RoundEnd = 10,

    TeamChanging = 11,  // when a user is changing his team in the game

    Ingame_BuyItem = 14,
    Ingame_PlayerSpawn = 20,

    // logging packet types
    OnGameEnd = 21,

    SetInventory =
        101,  // there are 2 or 3 other host packet types that send this
    ItemUsed = 105,
    SetLoadout = 107,
    SetBuyMenu = 111
};

#endif  // __PKT_HOSTSHARED_H_
