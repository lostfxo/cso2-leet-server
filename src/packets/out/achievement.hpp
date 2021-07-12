#ifndef __OUT_PKT_ACHIEVEMENT_
#define __OUT_PKT_ACHIEVEMENT_

#include "cso2/definitions.hpp"
#include "packets/builder.hpp"

class CampaignRewards;

class OutAchievementPacket
{
public:
    static PacketBuilder UpdateCampaign(cso2::CampaignFlags campaignId,
                                        const CampaignRewards& options);

private:
};

#endif  // __OUT_PKT_ACHIEVEMENT_
