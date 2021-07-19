#ifndef __CSO2_USER_H_
#define __CSO2_USER_H_

#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"
#include "util/json_fwd.hpp"

class UserService;

namespace cso2
{
class User
{
public:
    explicit User(const boost::json::value& jv);
    ~User() = default;

    inline bool operator==(const User& u) { return this->GetId() == u.GetId(); }

    inline std::uint32_t GetId() const { return this->m_Id; }

    inline std::string_view GetUserName() const { return this->m_UserName; }
    inline std::string_view GetPlayerName() const { return this->m_PlayerName; }
    inline std::string_view GetNetcafeName() const
    {
        return this->m_NetcafeName;
    }

    inline bool IsGameMaster() const { return this->m_GameMaster; }

    inline std::uint64_t GetPoints() const { return this->m_Points; }
    inline std::uint32_t GetCash() const { return this->m_Cash; }
    inline std::uint32_t GetMileage() const { return this->m_Mileage; }

    inline std::uint16_t GetLevel() const { return this->m_Level; }
    inline std::uint64_t GetCurXp() const { return this->m_CurXp; }
    inline std::uint64_t GetMaxXp() const { return this->m_MaxXp; }

    inline std::uint8_t GetVipLevel() const { return this->m_VipLevel; }
    inline std::uint32_t GetVipXp() const { return this->m_VipXp; }
    inline bool IsVip() const { return this->m_VipLevel > 0; }

    inline std::uint8_t GetRank() const { return this->m_Rank; }
    inline std::uint8_t GetRankFrame() const { return this->m_RankFrame; }

    inline std::uint32_t GetNumOfPlayedMatches() const
    {
        return this->m_PlayedMatches;
    }
    inline std::uint32_t GetWins() const { return this->m_Wins; }
    inline std::uint32_t GetSecondsPlayed() const
    {
        return this->m_SecondsPlayed;
    }

    inline std::uint32_t GetKills() const { return this->m_Kills; }
    inline std::uint32_t GetDeaths() const { return this->m_Deaths; }
    inline std::uint32_t GetAssists() const { return this->m_Assists; }
    inline std::uint32_t GetHeadshots() const { return this->m_Headshots; }
    inline std::uint16_t GetAccuracy() const { return this->m_Accuracy; }

    inline std::uint16_t GetAvatar() const { return this->m_Avatar; }
    inline const auto& GetUnlockedAvatars() const
    {
        return this->m_UnlockedAvatars;
    }

    inline std::uint16_t GetTitle() const { return this->m_Title; }
    inline const auto& GetUnlockedTitles() const
    {
        return this->m_UnlockedTitles;
    }

    inline std::string_view GetSignature() const { return this->m_Signature; }

    inline const auto& GetUnlockedAchievements() const
    {
        return this->m_UnlockedAchievements;
    }

    inline std::uint8_t GetNumOfUnreadMessages() const
    {
        return this->m_NumUnreadMessages;
    }

    inline CampaignFlags GetCampaignFlags() const
    {
        return this->m_CampaignFlags;
    }

    inline std::string_view GetClanName() const { return this->m_ClanName; }
    inline std::uint32_t GetClanMark() const { return this->m_ClanMark; }

    inline std::uint32_t GetWorldRank() const { return this->m_WorldRank; }

    inline std::uint32_t GetBestGamemode() const
    {
        return this->m_BestGamemode;
    }
    inline std::uint32_t GetBestMap() const { return this->m_BestMap; }

    inline std::uint64_t GetSkillHumanCurXp() const
    {
        return this->m_SkillHumanCurXp;
    }
    inline std::uint64_t GetSkillHumanMaxXp() const
    {
        return this->m_SkillHumanMaxXp;
    }
    inline std::uint8_t GetSkillHumanPoints() const
    {
        return this->m_SkillHumanPoints;
    }

    inline std::uint64_t GetSkillZombieCurXp() const
    {
        return this->m_SkillZombieCurXp;
    }
    inline std::uint64_t GetSkillZombieMaxXp() const
    {
        return this->m_SkillZombieMaxXp;
    }
    inline std::uint8_t GetSkillZombiePoints() const
    {
        return this->m_SkillZombiePoints;
    }

    inline bool ShouldForceRelayAsHost() const
    {
        return this->m_ForceRelayHost;
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

    bool m_ForceRelayHost;

    friend ::UserService;
};

}  // namespace cso2

#endif  // __CSO2_USER_H_
