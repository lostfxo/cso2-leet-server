#ifndef __IN_PACKET_HOST_ONPLAYERSPAWN_H_
#define __IN_PACKET_HOST_ONPLAYERSPAWN_H_

#include <cstdint>

#include "cso2/vector.hpp"

class PacketView;

class InHostPacketOnPlayerSpawn
{
public:
    InHostPacketOnPlayerSpawn(PacketView& packet);

    std::uint32_t PlayerUserId;
    IntVector SpawnPoint;
    bool Spectating;
};

#endif  // __IN_PACKET_HOST_ONPLAYERSPAWN_H_
