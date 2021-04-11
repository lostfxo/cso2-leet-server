#include "packets/in/host/onplayerspawn.hpp"

#include "packets/view.hpp"

InHostPacketOnPlayerSpawn::InHostPacketOnPlayerSpawn(PacketView& packet)
{
    this->PlayerUserId = packet.Read<std::uint32_t>();
    this->SpawnPoint = IntVector(packet.Read<int32_t>(), packet.Read<int32_t>(),
                                 packet.Read<int32_t>());
    this->Spectating = packet.Read<std::uint8_t>() == 1;
}
