#include "packets/in/userprofile/updatecampaign.hpp"

#include "packets/view.hpp"

InUserProfilePacketUpdateCampaign::InUserProfilePacketUpdateCampaign(
    PacketView& packet)
{
    this->Type = UP_UpdateCampaignType(packet.Read<std::uint8_t>());

    if (this->Type == UP_UpdateCampaignType::Finished)
    {
        this->CampaignId = cso2::CampaignFlags(packet.Read<std::uint16_t>());
    }
}
