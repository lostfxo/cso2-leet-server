#ifndef __PACKETLOGGER_H_
#define __PACKETLOGGER_H_

#include <span>
#include <string_view>

class PacketLogger
{
public:
    static void Init();

    static void OnInPacket(std::string_view connUuid, std::uint32_t seq,
                           std::uint32_t packetId,
                           const std::span<const std::uint8_t> packetData);
    static void OnOutPacket(std::string_view connUuid, std::uint32_t seq,
                            std::uint32_t packetId,
                            const std::span<const std::uint8_t> packetData);
};

#endif  // __PACKETLOGGER_H_
