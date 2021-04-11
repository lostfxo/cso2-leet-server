#ifndef __IN_PACKET_USERPROFILE_H_
#define __IN_PACKET_USERPROFILE_H_

#include "packets/userprofileshared.hpp"

class PacketView;

class InUserProfilePacket
{
public:
    InUserProfilePacket(PacketView& packet);

    UserProfilePacketType Type;
};

#endif  // __IN_PACKET_USERPROFILE_H_
