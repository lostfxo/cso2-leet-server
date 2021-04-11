#ifndef __ROOM_SHARED_H_
#define __ROOM_SHARED_H_

#include <list>
#include <memory>
#include <string>

#include "cso2/definitions.hpp"

class Room;
class RoomSettings;
class Slot;

enum class RoomCountdownType : std::uint8_t
{
    Progress = 0,
    Stop = 1
};

enum class RoomStatus : std::uint8_t
{
    Waiting = 1,
    Ingame = 2
};

struct NewRoomRequestOptions
{
    std::string RoomName;
    std::string Password;

    std::uint16_t unk00;
    std::uint8_t unk01;
    cso2::GameMode GameModeId;
    std::uint8_t MapId;
    std::uint8_t WinLimit;
    std::uint16_t KillLimit;
    std::uint8_t unk02;
    std::uint8_t unk03;
    std::uint8_t unk04;
    std::uint8_t unk06;
    std::uint8_t unk07;
    std::uint8_t unk08;
    std::uint8_t unk09;
    std::uint8_t unk10;
    std::uint32_t unk11;
};

using RoomPtr = std::shared_ptr<Room>;
using RoomList = std::list<RoomPtr>;

#endif  // __ROOM_SHARED_H_
