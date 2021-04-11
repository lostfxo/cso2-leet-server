#include "handlers.hpp"

#include "clientsession.hpp"
#include "cso2/definitions.hpp"
#include "cso2/user.hpp"
#include "packets/achievementshared.hpp"
#include "packets/view.hpp"
#include "util/log.hpp"

#include "packets/in/achievement.hpp"
#include "packets/in/achievement/campaign.hpp"
#include "packets/out/achievement.hpp"

void OnAchievementCampaignInfoRequest(PacketView& packet,
                                      ClientSessionPtr session)
{
    auto campaignPkt = InAchievementPacketCampaign(packet);

    CampaignRewards rewardOptions;

    switch (campaignPkt.TargetCampaign)
    {
        case cso2::CampaignFlags::Campaign_0:
            break;
        case cso2::CampaignFlags::Campaign_1:
            rewardOptions.SetRewardXp(3000);
            break;
        case cso2::CampaignFlags::Campaign_2:
            rewardOptions.SetRewardPoints(5000);
            break;
        case cso2::CampaignFlags::Campaign_3:
            rewardOptions.SetRewardIcon(24);
            break;
        case cso2::CampaignFlags::Campaign_4:
            rewardOptions.SetRewardItems(
                { { .ItemId = 1002, .Ammount = 1, .TimeLimit = 0 },
                  { .ItemId = 1004, .Ammount = 1, .TimeLimit = 0 } });
            break;
        case cso2::CampaignFlags::Campaign_5:
            rewardOptions.SetRewardItems(
                { { .ItemId = 54, .Ammount = 1, .TimeLimit = 0 },
                  { .ItemId = 55, .Ammount = 1, .TimeLimit = 0 } });
            break;
        case cso2::CampaignFlags::Campaign_6:
            // unused
            break;
        default:
            Log::Warning(
                "[OnAchievementCampaignInfoRequest] user {} sent campaign "
                "packet with unknown campaign {}",
                session->GetUser()->GetId(), campaignPkt.TargetCampaign);
            return;
    }

    auto outPkt = OutAchievementPacket::UpdateCampaign(
        campaignPkt.TargetCampaign, rewardOptions);
    session->Send(std::move(outPkt));
}

void OnAchievementPacketRequest(PacketView& packet, ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnAchievementPacketRequest] conn '{}' sent an "
                     "achievement packet without session\n",
                     session->GetUniqueId());
        return;
    }

    auto achPacket = InAchievementPacket(packet);

    switch (achPacket.Type)
    {
        case AchievementPacketType::Campaign:
            OnAchievementCampaignInfoRequest(packet, session);
            break;
        default:
            Log::Warning("[OnAchievementPacketRequest] connection {} sent an "
                         "unknown {} achievement packet\n",
                         session->GetUniqueId(), achPacket.Type);
    }
}
