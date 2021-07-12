#include "packets/in/host/onroundend.hpp"

#include "packets/view.hpp"

InHostPacketOnRoundEnd::InHostPacketOnRoundEnd(PacketView& packet)
{
    this->WinnerTeam = cso2::TeamNum(packet.Read<std::uint8_t>());
    this->TotalTerWins = packet.Read<std::uint8_t>();
    this->TotalCtWins = packet.Read<std::uint8_t>();
    this->unk = packet.Read<std::uint8_t>();
}
