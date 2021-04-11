#ifndef __IN_PACKET_HOST_ITEMUSED_H_
#define __IN_PACKET_HOST_ITEMUSED_H_

#include "cso2/definitions.hpp"

class PacketView;

class InHostPacketItemUsed
{
public:
    InHostPacketItemUsed(PacketView& packet);

    std::uint32_t UserId;
    std::uint32_t ItemId;
};

#endif  // __IN_PACKET_HOST_ITEMUSED_H_
