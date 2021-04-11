#ifndef __CSO2_USER_H_
#define __CSO2_USER_H_

#include <string>
#include <string_view>

#include <boost/json/value.hpp>

#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"

namespace json = boost::json;

class UserService;

namespace cso2
{
class User
{
public:
    User() = delete;
    User(const User&) = delete;

    explicit User(const json::value& jv);
    ~User() = default;

    inline bool operator==(const User& u) { return this->GetId() == u.GetId(); }

    [[nodiscard]] inline std::uint32_t GetId() const noexcept { return this->m_Id; }

    [[nodiscard]] inline std::string_view GetUserName() const noexcept
    {
        return this->m_UserName;
    }
    [[nodiscard]] inline std::string_view GetPlayerName() const noexcept
    {
        return this->m_PlayerName;
    }
    [[nodiscard]] inline std::string_view GetNetcafeName() const noexcept
    {
        return this->m_NetcafeName;
    }

    [[nodiscard]] inline bool IsGameMaster() const noexcept
    {
        return this->m_GameMaster;
    }

    [[nodiscard]] inline std::uint64_t GetPoints() const noexcept
    {
        return this->m_Points;
    }
    [[nodiscard]] inline std::uint32_t GetCash() const noexcept
    {
        return this->m_Cash;
    }
    [[nodiscard]] inline std::uint32_t GetMileage() const noexcept
    {
        return this->m_Mileage;
    }

    [[nodiscard]] inline std::uint16_t GetLevel() const noexcept
    {
        return this->m_Level;
    }
    [[nodiscard]] inline std::uint64_t GetCurXp() const noexcept
    {
        return this->m_CurXp;
    }
    [[nodiscard]] inline std::uint64_t GetMaxXp() const noexcept
    {
        return this->m_MaxXp;
    }

    [[nodiscard]] inline std::uint8_t GetVipLevel() const noexcept
    {
        return this->m_VipLevel;
    }
    [[nodiscard]] inline std::uint32_t GetVipXp() const noexcept
    {
        return this->m_VipXp;
    }
    [[nodiscard]] inline bool IsVip() const noexcept
    {
        return this->m_VipLevel > 0;
    }

    [[nodiscard]] inline std::uint8_t GetRank() const noexcept
    {
        return this->m_Rank;
    }
    [[nodiscard]] inline std::uint8_t GetRankFrame() const noexcept
    {
        return this->m_RankFrame;
    }

    [[nodiscard]] inline std::uint32_t GetNumOfPlayedMatches() const noexcept
    {
        return this->m_PlayedMatches;
    }
    [[nodiscard]] inline std::uint32_t GetWins() const noexcept
    {
        return this->m_Wins;
    }
    [[nodiscard]] inline std::uint32_t GetSecondsPlayed() const noexcept
    {
        return this->m_SecondsPlayed;
    }

    [[nodiscard]] inline std::uint32_t GetKills() const noexcept
    {
        return this->m_Kills;
    }
    [[nodiscard]] inline std::uint32_t GetDeaths() const noexcept
    {
        return this->m_Deaths;
    }
    [[nodiscard]] inline std::uint32_t GetAssists() const noexcept
    {
        return this->m_Assists;
    }
    [[nodiscard]] inline std::uint32_t GetHeadshots() const noexcept
    {
        return this->m_Headshots;
    }
    [[nodiscard]] inline std::uint16_t GetAccuracy() const noexcept
    {
        return this->m_Accuracy;
    }

    [[nodiscard]] inline std::uint16_t GetAvatar() const noexcept
    {
        return this->m_Avatar;
    }
    [[nodiscard]] inline const auto& GetUnlockedAvatars() const noexcept
    {
        return this->m_UnlockedAvatars;
    }

    [[nodiscard]] inline std::uint16_t GetTitle() const noexcept
    {
        return this->m_Title;
    }
    [[nodiscard]] inline const auto& GetUnlockedTitles() const noexcept
    {
        return this->m_UnlockedTitles;
    }

    [[nodiscard]] inline std::string_view GetSignature() const noexcept
    {
        return this->m_Signature;
    }

    [[nodiscard]] inline const auto& GetUnlockedAchievements() const noexcept
    {
        return this->m_UnlockedAchievements;
    }

    [[nodiscard]] inline std::uint8_t GetNumOfUnreadMessages() const noexcept
    {
        return this->m_NumUnreadMessages;
    }

    [[nodiscard]] inline CampaignFlags GetCampaignFlags() const noexcept
    {
        return this->m_CampaignFlags;
    }

    [[nodiscard]] inline std::string_view GetClanName() const noexcept
    {
        return this->m_ClanName;
    }
    [[nodiscard]] inline std::uint32_t GetClanMark() const noexcept
    {
        return this->m_ClanMark;
    }

    [[nodiscard]] inline std::uint32_t GetWorldRank() const noexcept
    {
        return this->m_WorldRank;
    }

    [[nodiscard]] inline std::uint32_t GetBestGamemode() const noexcept
    {
        return this->m_BestGamemode;
    }
    [[nodiscard]] inline std::uint32_t GetBestMap() const noexcept
    {
        return this->m_BestMap;
    }

    [[nodiscard]] inline std::uint64_t GetSkillHumanCurXp() const noexcept
    {
        return this->m_SkillHumanCurXp;
    }
    [[nodiscard]] inline std::uint64_t GetSkillHumanMaxXp() const noexcept
    {
        return this->m_SkillHumanMaxXp;
    }
    [[nodiscard]] inline std::uint8_t GetSkillHumanPoints() const noexcept
    {
        return this->m_SkillHumanPoints;
    }

    [[nodiscard]] inline std::uint64_t GetSkillZombieCurXp() const noexcept
    {
        return this->m_SkillZombieCurXp;
    }
    [[nodiscard]] inline std::uint64_t GetSkillZombieMaxXp() const noexcept
    {
        return this->m_SkillZombieMaxXp;
    }
    [[nodiscard]] inline std::uint8_t GetSkillZombiePoints() const noexcept
    {
        return this->m_SkillZombiePoints;
    }

private:
    // these properties were aligned this way they so they use less memory
    std::uint32_t m_Id;
    std::string m_UserName;
    std::string m_PlayerName;

    std::string m_Signature;
    std::string m_NetcafeName;

    std::uint64_t m_Points;
    std::uint32_t m_Cash;
    std::uint32_t m_Mileage;

    std::uint64_t m_CurXp;
    std::uint64_t m_MaxXp;

    std::uint32_t m_VipXp;

    std::uint32_t m_PlayedMatches;
    std::uint32_t m_Wins;
    std::uint32_t m_SecondsPlayed;

    std::uint32_t m_Kills;
    std::uint32_t m_Deaths;
    std::uint32_t m_Assists;
    std::uint32_t m_Headshots;

    std::string m_ClanName;
    std::uint32_t m_ClanMark;

    std::uint32_t m_WorldRank;

    std::uint32_t m_BestGamemode;
    std::uint32_t m_BestMap;

    std::uint64_t m_SkillHumanCurXp;
    std::uint64_t m_SkillHumanMaxXp;
    std::uint64_t m_SkillZombieCurXp;
    std::uint64_t m_SkillZombieMaxXp;

    std::array<std::uint8_t, 128> m_UnlockedAvatars;
    std::array<std::uint8_t, 128> m_UnlockedTitles;
    std::array<std::uint8_t, 128> m_UnlockedAchievements;

    std::uint16_t m_Accuracy;  // hit rate

    CampaignFlags m_CampaignFlags;

    std::uint16_t m_Level;

    std::uint16_t m_Avatar;
    std::uint16_t m_Title;

    std::uint8_t m_VipLevel;

    std::uint8_t m_SkillHumanPoints;
    std::uint8_t m_SkillZombiePoints;

    std::uint8_t m_Rank;
    std::uint8_t m_RankFrame;

    std::uint8_t m_NumUnreadMessages;  // TODO: what is this for?

    bool m_GameMaster;

    friend ::UserService;
};

}  // namespace cso2

#endif  // __CSO2_USER_H_
