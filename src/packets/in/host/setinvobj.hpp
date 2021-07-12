#ifndef __IN_PACKET_HOST_SETINVOBJ_H_
#define __IN_PACKET_HOST_SETINVOBJ_H_

#include <cstdint>

class PacketView;

class InHostPacketSetInvObject
{
public:
    InHostPacketSetInvObject(PacketView& packet);

    std::uint32_t UserId;
};

#endif  // __IN_PACKET_HOST_SETINVOBJ_H_
