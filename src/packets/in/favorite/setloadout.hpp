#ifndef __IN_PACKET_FAVORITE_SETLOADOUT_H_
#define __IN_PACKET_FAVORITE_SETLOADOUT_H_

#include "cso2/definitions.hpp"

class PacketView;

class InFavoritePacketSetLoadout
{
public:
    InFavoritePacketSetLoadout(PacketView& packet);

    std::uint32_t ItemId;
    std::uint8_t LoadoutNum;
    cso2::LoadoutSlot WeaponSlot;
};

#endif  // __IN_PACKET_FAVORITE_SETLOADOUT_H_
