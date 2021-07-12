#ifndef __PACKET_LOGIN_H_
#define __PACKET_LOGIN_H_

#include <array>
#include <cstdint>
#include <string>

#include "packets/view.hpp"

class InLoginPacket
{
public:
    InLoginPacket(PacketView& packet)
    {
        this->NexonUsername = packet.ReadString();
        this->GameUsername = packet.ReadString();
        this->_unk00 = packet.Read<std::uint8_t>();
        this->Password = packet.ReadLongString();
        this->HddHwid = packet.ReadArray<std::uint8_t, 16>();
        this->NetCafeId = std::to_string(packet.Read<std::uint32_t>(true));
        this->_unk01 = packet.Read<std::uint32_t>();
        this->UserSn = packet.Read<std::uint64_t>();
        this->_unk02 = packet.ReadLongString();
        this->_unk03 = packet.Read<std::uint8_t>();
        this->IsLeague = packet.Read<std::uint8_t>();
        this->_unk04 = packet.Read<std::uint8_t>();
        this->_unk05 = packet.ReadString();
        this->_unk06 = packet.ReadString();
        this->_unk07 = packet.ReadString();
    }

    std::string NexonUsername;
    std::string GameUsername;

    std::string Password;

    std::array<std::uint8_t, 16> HddHwid;
    std::string NetCafeId;

    std::string _unk02;
    std::string _unk05;
    std::string _unk06;
    std::string _unk07;

    std::uint64_t UserSn;

    std::uint32_t _unk01;

    std::uint8_t _unk00;
    std::uint8_t _unk03;
    bool IsLeague;
    std::uint8_t _unk04;
};

#endif  // __PACKET_LOGIN_H_
