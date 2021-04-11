#ifndef __PACKETS_ACHIEVEMENTSHARED_H_
#define __PACKETS_ACHIEVEMENTSHARED_H_

#include <cstdint>
#include <vector>

enum class AchievementPacketType : std::uint8_t
{
    Campaign = 3,
    Boss = 4
};

struct CampaignRewardItem
{
    std::uint32_t ItemId;
    std::uint16_t Ammount;
    std::uint16_t TimeLimit;  // TODO: or is this a boolean that tells that the
                              // item is time limited?
};

using CampaignRewardItemsArray = std::vector<CampaignRewardItem>;

class CampaignRewards
{
public:
    CampaignRewards();

    [[nodiscard]] inline std::uint32_t GetFlags() const noexcept
    {
        return this->m_Flags;
    }

    [[nodiscard]] inline std::uint16_t GetUnk() const noexcept
    {
        return this->m_unk;
    }

    [[nodiscard]] inline std::uint32_t GetUnk2() const noexcept
    {
        return this->m_unk2;
    }

    [[nodiscard]] inline std::uint16_t GetRewardTitle() const noexcept
    {
        return this->m_RewardTitle;
    }

    [[nodiscard]] inline std::uint16_t GetRewardIcon() const noexcept
    {
        return this->m_RewardIcon;
    }

    [[nodiscard]] inline std::uint32_t GetRewardPoints() const noexcept
    {
        return this->m_RewardPoints;
    }

    [[nodiscard]] inline std::uint32_t GetRewardXp() const noexcept
    {
        return this->m_RewardXp;
    }

    [[nodiscard]] inline const CampaignRewardItemsArray& GetRewardItems()
        const noexcept
    {
        return this->m_RewardItems;
    }

    [[nodiscard]] inline std::uint16_t GetUnk3() const noexcept
    {
        return this->m_unk3;
    }

    void SetUnk(std::uint16_t newUnk) noexcept;
    void SetUnk2(std::uint32_t newUnk2) noexcept;
    void SetRewardTitle(std::uint16_t newRewardTitle) noexcept;
    void SetRewardIcon(std::uint16_t newRewardIcon) noexcept;
    void SetRewardPoints(std::uint32_t newRewardPoints) noexcept;
    void SetRewardXp(std::uint32_t newRewardXp) noexcept;
    void SetRewardItems(CampaignRewardItemsArray&& newRewardItems);
    void SetUnk3(std::uint16_t newUnk3) noexcept;

private:
    std::uint32_t m_Flags;

    // members are memory aligned
    CampaignRewardItemsArray m_RewardItems;
    std::uint32_t m_unk2;
    std::uint32_t m_RewardPoints;
    std::uint32_t m_RewardXp;
    std::uint16_t m_unk;
    std::uint16_t m_RewardTitle;
    std::uint16_t m_RewardIcon;
    std::uint16_t m_unk3;
};

#endif  // __PACKETS_ACHIEVEMENTSHARED_H_
