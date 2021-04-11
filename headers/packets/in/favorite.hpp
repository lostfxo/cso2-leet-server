#ifndef __IN_PACKET_FAVORITE_H_
#define __IN_PACKET_FAVORITE_H_

#include "packets/favoriteshared.hpp"

class PacketView;

class InFavoritePacket
{
public:
    InFavoritePacket(PacketView& packet);

    FavoritePacketType Type;
};

#endif  // __IN_PACKET_FAVORITE_H_
