#include "packets/in/achievement/campaign.hpp"

#include "packets/view.hpp"

InAchievementPacketCampaign::InAchievementPacketCampaign(PacketView& packet)
{
    this->TargetCampaign = cso2::CampaignFlags(packet.Read<std::uint16_t>());
}
