#include "packets/in/host/itemused.hpp"

#include "packets/view.hpp"

InHostPacketItemUsed::InHostPacketItemUsed(PacketView& packet)
{
    this->UserId = packet.Read<std::uint32_t>();
    this->ItemId = packet.Read<std::uint32_t>();
}
