#ifndef __IN_PACKET_HOST_ONPLAYERKILLED_H_
#define __IN_PACKET_HOST_ONPLAYERKILLED_H_

#include "cso2/takedamageinfo.hpp"

class PacketView;

class InHostPacketOnPlayerKilled
{
public:
    InHostPacketOnPlayerKilled(PacketView& packet);

    cso2::TakeDamageInfo DamageInfo;
};

#endif  // __IN_PACKET_HOST_ONPLAYERKILLED_H_
