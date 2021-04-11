#ifndef __IN_PACKET_ACHIEVEMENT_H_
#define __IN_PACKET_ACHIEVEMENT_H_

#include "packets/achievementshared.hpp"

class PacketView;

class InAchievementPacket
{
public:
    InAchievementPacket(PacketView& packet);

    AchievementPacketType Type;
};

#endif  // __IN_PACKET_ACHIEVEMENT_H_
