#ifndef __HOLEPUNCH_SHARED_H__
#define __HOLEPUNCH_SHARED_H__

#include <cstdint>

constexpr const std::uint8_t HOLEPUNCH_PACKET_SIGNATURE = 0x57;  // 'W'

enum class HolepunchPacketType : std::uint8_t
{
    Punch = 0,
    HeartbeatForClient = 1,
    HeartbeatForServer = 2,
    HeartbeatForSourceTV = 3
};

enum class HolepunchPortId : std::uint8_t
{
    Client = 0,
    Server = 1,
    SourceTV = 2
};

#endif  // __HOLEPUNCH_SHARED_H__
