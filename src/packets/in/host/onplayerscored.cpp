#include "packets/in/host/onplayerscored.hpp"

#include "packets/view.hpp"

InHostPacketOnPlayerScored::InHostPacketOnPlayerScored(PacketView& packet)
{
    this->FakeClient = packet.Read<std::uint8_t>() == 1;
    this->ScorerUserId = packet.Read<std::uint32_t>();
    this->unk03 = packet.Read<std::uint8_t>();
    this->unk04 = packet.Read<std::uint32_t>();
    this->NewFragsCount = packet.Read<std::uint8_t>();
    this->TotalFrags = packet.Read<std::uint16_t>();
    this->ScorerTeam = cso2::TeamNum(packet.Read<std::uint8_t>());
}
