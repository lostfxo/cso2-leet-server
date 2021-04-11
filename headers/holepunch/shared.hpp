#ifndef __HOLEPUNCH_SHARED_H__
#define __HOLEPUNCH_SHARED_H__

#include <cstdint>

constexpr const std::uint8_t HOLEPUNCH_PACKET_SIGNATURE = 0x57;  // 'W'

enum class HolepunchType : std::uint16_t
{
    Client = 0,
    Server = 256,
    SourceTV = 512
};

#endif  // __HOLEPUNCH_SHARED_H__
