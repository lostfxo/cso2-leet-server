#include "packets/in/favorite.hpp"

#include "packets/view.hpp"

InFavoritePacket::InFavoritePacket(PacketView& packet)
{
    this->Type = static_cast<FavoritePacketType>(packet.Read<std::uint8_t>());
}
