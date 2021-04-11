#ifndef __IN_PACKET_FAVORITE_SETCOSMETICS_H_
#define __IN_PACKET_FAVORITE_SETCOSMETICS_H_

#include "cso2/definitions.hpp"

class PacketView;

class InFavoritePacketSetCosmetics
{
public:
    InFavoritePacketSetCosmetics(PacketView& packet);

    std::uint32_t ItemId;
    cso2::CosmeticSlot Slot;
};

#endif  // __IN_PACKET_FAVORITE_SETCOSMETICS_H_
