#ifndef __PACKETLOGGER_H_
#define __PACKETLOGGER_H_

#include "util/fs.hpp"

class PacketLogger
{
public:
    PacketLogger();
    ~PacketLogger() = default;

    PacketLogger(const PacketLogger&) = delete;
    PacketLogger& operator=(const PacketLogger&) = delete;

    void OnInPacket(std::string_view connUuid, std::uint32_t seq, std::uint32_t packetId,
                    const std::span<const std::uint8_t> packetData) const;
    void OnOutPacket(std::string_view connUuid, std::uint32_t seq, std::uint32_t packetId,
                     const std::span<const std::uint8_t> packetData) const;

private:
    fs::path m_InPath;
    fs::path m_OutPath;
};

#endif  // __PACKETLOGGER_H_
