#ifndef __PACKETS_FAVORITESHARED_H_
#define __PACKETS_FAVORITESHARED_H_

#include <cstdint>

enum class FavoritePacketType : std::uint8_t
{
    SetLoadout = 0,
    SetCosmetics = 1
};

#endif  // __PACKETS_FAVORITESHARED_H_
