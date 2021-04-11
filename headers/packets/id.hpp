#ifndef __PACKET_ID_H_
#define __PACKET_ID_H_

#include <cstdint>

constexpr const std::uint8_t GAME_PACKET_SIGNATURE = 'U';
constexpr const std::uint32_t BASE_PACKET_HEADER_LENGTH = 4;
constexpr const std::uint32_t GAME_PACKET_HEADER_LENGTH = 5;

enum class PacketId : std::uint8_t
{
    Invalid = std::uint8_t(-1),
    Version = 0,
    Reply = 1,
    Login = 3,
    ServerList = 5,
    Character = 6,
    RequestRoomList = 7,
    RequestChannels = 10,
    Room = 65,
    Chat = 67,
    Host = 68,
    UserProfile = 69,
    Udp = 70,
    Shop = 72,
    Ban = 74,
    Option = 76,
    Favorite = 77,
    QuickStart = 86,
    Automatch = 88,
    Friend = 89,
    Unlock = 90,
    UnReaded_Message = 91,
    GZ = 95,
    Achievement = 96,
    ConfigInfo = 106,
    Lobby = 107,
    UserStart = 150,
    RoomList = 151,
    Inventory_Add = 152,
    Inventory_Create = 154,
    UserInfo = 157
};

#endif  // __PACKET_ID_H_
