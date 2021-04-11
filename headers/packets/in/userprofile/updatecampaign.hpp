#ifndef __IN_PACKETS_USERPROFILE_UPDATECAMPAIGN_H_
#define __IN_PACKETS_USERPROFILE_UPDATECAMPAIGN_H_

#include "cso2/definitions.hpp"
#include "packets/userprofileshared.hpp"

class PacketView;

class InUserProfilePacketUpdateCampaign
{
public:
    InUserProfilePacketUpdateCampaign(PacketView& packet);

    UP_UpdateCampaignType Type;
    cso2::CampaignFlags CampaignId;
};

#endif  // __IN_PACKETS_USERPROFILE_UPDATECAMPAIGN_H_
