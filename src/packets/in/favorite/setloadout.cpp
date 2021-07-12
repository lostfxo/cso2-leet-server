#include "packets/in/favorite/setloadout.hpp"

#include "packets/view.hpp"

InFavoritePacketSetLoadout::InFavoritePacketSetLoadout(PacketView& packet)
{
    this->LoadoutNum = packet.Read<std::uint8_t>();
    this->WeaponSlot = static_cast<cso2::LoadoutSlot>(packet.Read<std::uint8_t>());
    this->ItemId = packet.Read<std::uint32_t>();
}
