#include "packets/in/host/changeteam.hpp"

#include "packets/view.hpp"

InHostPacketChangeTeam::InHostPacketChangeTeam(PacketView& packet)
{
    // this->Type = packet.Read<std::uint8_t>();
    this->UserId = packet.Read<std::uint16_t>();
    this->unk00 = packet.Read<std::uint8_t>();
    this->unk01 = packet.Read<std::uint8_t>();
    this->NewTeam = cso2::TeamNum(packet.Read<std::uint8_t>());
}
