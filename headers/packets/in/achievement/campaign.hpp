#ifndef __IN_PACKET_ACHIEVEMENT_CAMPAIGN_H_
#define __IN_PACKET_ACHIEVEMENT_CAMPAIGN_H_

#include "cso2/definitions.hpp"

class PacketView;

class InAchievementPacketCampaign
{
public:
    InAchievementPacketCampaign(PacketView& packet);

    cso2::CampaignFlags TargetCampaign;
};

#endif  // __IN_PACKET_ACHIEVEMENT_CAMPAIGN_H_
