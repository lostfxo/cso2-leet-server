#ifndef __PACKET_VERSION_H_
#define __PACKET_VERSION_H_

#include <cstdint>
#include <string>

#include "packets/view.hpp"

class InVersionPacket
{
public:
    InVersionPacket(PacketView& packet)
    {
        this->unk = packet.Read<std::uint8_t>();
        this->unk2 = packet.Read<std::uint16_t>();
        this->ClientHash = packet.ReadString();
    }

    std::uint32_t unk;
    std::uint32_t unk2;
    std::string ClientHash;
};

#endif  // __PACKET_VERSION_H_
