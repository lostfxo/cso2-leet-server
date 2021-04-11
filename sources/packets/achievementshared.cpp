#include "packets/achievementshared.hpp"

CampaignRewards::CampaignRewards() : m_Flags(0) {}

void CampaignRewards::SetUnk(std::uint16_t newUnk) noexcept
{
    this->m_unk = newUnk;
    this->m_Flags |= 0x1;
}

void CampaignRewards::SetUnk2(std::uint32_t newUnk2) noexcept
{
    this->m_unk2 = newUnk2;
    this->m_Flags |= 0x2;
}

void CampaignRewards::SetRewardTitle(std::uint16_t newRewardTitle) noexcept
{
    this->m_RewardTitle = newRewardTitle;
    this->m_Flags |= 0x4;
}

void CampaignRewards::SetRewardIcon(std::uint16_t newRewardIcon) noexcept
{
    this->m_RewardIcon = newRewardIcon;
    this->m_Flags |= 0x8;
}

void CampaignRewards::SetRewardPoints(std::uint32_t newRewardPoints) noexcept
{
    this->m_RewardPoints = newRewardPoints;
    this->m_Flags |= 0x10;
}

void CampaignRewards::SetRewardXp(std::uint32_t newRewardXp) noexcept
{
    this->m_RewardXp = newRewardXp;
    this->m_Flags |= 0x20;
}

void CampaignRewards::SetRewardItems(CampaignRewardItemsArray&& newRewardItems)
{
    this->m_RewardItems = std::move(newRewardItems);
    this->m_Flags |= 0x40;
}

void CampaignRewards::SetUnk3(std::uint16_t newUnk3) noexcept
{
    this->m_unk3 = newUnk3;
    this->m_Flags |= 0x80;
}
