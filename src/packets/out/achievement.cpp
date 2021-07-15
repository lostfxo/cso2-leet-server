#include "packets/out/achievement.hpp"

#include "util/number.hpp"

#include "packets/achievementshared.hpp"

PacketBuilder OutAchievementPacket::UpdateCampaign(
    cso2::CampaignFlags campaignId, const CampaignRewards& options)
{
    PacketBuilder res(PacketId::Achievement, 256);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(AchievementPacketType::Campaign));
    bufRef.Write(std::uint16_t(campaignId));

    auto flags = options.GetFlags();
    bufRef.Write<std::uint32_t>(flags);

    if (flags & 0x1)
    {
        bufRef.Write<std::uint16_t>(options.GetUnk());
    }

    if (flags & 0x2)
    {
        bufRef.Write<std::uint32_t>(options.GetUnk2());
    }

    if (flags & 0x4)
    {
        bufRef.Write<std::uint16_t>(options.GetRewardTitle());
    }

    if (flags & 0x8)
    {
        bufRef.Write<std::uint16_t>(options.GetRewardIcon());
    }

    if (flags & 0x10)
    {
        bufRef.Write<std::uint32_t>(options.GetRewardPoints());
    }

    if (flags & 0x20)
    {
        bufRef.Write<std::uint32_t>(options.GetRewardXp());
    }

    if (flags & 0x40)
    {
        auto items = options.GetRewardItems();
        bufRef.Write(util::FastNarrow<uint8_t>(items.size()));

        for (const auto& item : items)
        {
            bufRef.Write<std::uint32_t>(item.ItemId);
            bufRef.Write<std::uint16_t>(item.Ammount);
            bufRef.Write<std::uint16_t>(item.TimeLimit);
        }
    }

    if (flags & 0x80)
    {
        bufRef.Write<std::uint16_t>(options.GetUnk3());
    }

    return res;
}
