#include "packets/in/favorite/setcosmetics.hpp"

#include "packets/view.hpp"

InFavoritePacketSetCosmetics::InFavoritePacketSetCosmetics(PacketView& packet)
{
    this->Slot = static_cast<cso2::CosmeticSlot>(packet.Read<std::uint8_t>());
    this->ItemId = packet.Read<std::uint32_t>();
}
