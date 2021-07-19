#include "cso2/user.hpp"

#include <boost/json/value.hpp>

#include "util/json_conv.hpp"
#include "util/number.hpp"

namespace cso2
{
User::User(const boost::json::value& jv)
{
    this->m_Id = util::FastNarrow<std::uint32_t>(jv.at("id").as_int64());
    this->m_UserName = jv.at("username").as_string();
    this->m_PlayerName = jv.at("playername").as_string();

    this->m_GameMaster = jv.at("gm").as_bool();

    this->m_Points =
        util::FastNarrow<std::uint64_t>(jv.at("points").as_int64());
    this->m_Cash = util::FastNarrow<std::uint32_t>(jv.at("cash").as_int64());
    this->m_Mileage =
        util::FastNarrow<std::uint32_t>(jv.at("mpoints").as_int64());

    this->m_Level = util::FastNarrow<std::uint16_t>(jv.at("level").as_int64());
    this->m_CurXp = JStrToUint64(jv.at("cur_xp").as_string());
    this->m_MaxXp = JStrToUint64(jv.at("max_xp").as_string());
    this->m_VipLevel =
        util::FastNarrow<std::uint8_t>(jv.at("vip_level").as_int64());
    this->m_VipXp = util::FastNarrow<std::uint32_t>(jv.at("vip_xp").as_int64());

    this->m_Rank = util::FastNarrow<std::uint8_t>(jv.at("rank").as_int64());
    this->m_RankFrame =
        util::FastNarrow<std::uint8_t>(jv.at("rank_frame").as_int64());

    this->m_PlayedMatches =
        util::FastNarrow<std::uint32_t>(jv.at("played_matches").as_int64());
    this->m_Wins = util::FastNarrow<std::uint32_t>(jv.at("wins").as_int64());
    this->m_SecondsPlayed =
        util::FastNarrow<std::uint32_t>(jv.at("seconds_played").as_int64());

    this->m_Kills = util::FastNarrow<std::uint32_t>(jv.at("kills").as_int64());
    this->m_Deaths =
        util::FastNarrow<std::uint32_t>(jv.at("deaths").as_int64());
    this->m_Assists =
        util::FastNarrow<std::uint32_t>(jv.at("assists").as_int64());
    this->m_Headshots =
        util::FastNarrow<std::uint32_t>(jv.at("headshots").as_int64());
    this->m_Accuracy =
        util::FastNarrow<std::uint16_t>(jv.at("accuracy").as_int64());

    auto arrayConversion = [](const json::value v) -> int64_t
    { return v.as_int64(); };

    this->m_Avatar =
        util::FastNarrow<std::uint16_t>(jv.at("avatar").as_int64());
    const auto& unlockedAvatars = jv.at("unlocked_avatars").as_array();
    std::transform(unlockedAvatars.begin(), unlockedAvatars.end(),
                   this->m_UnlockedAvatars.begin(), arrayConversion);

    this->m_Title = util::FastNarrow<std::uint16_t>(jv.at("title").as_int64());
    const auto& unlockedTitles = jv.at("unlocked_titles").as_array();
    std::transform(unlockedTitles.begin(), unlockedTitles.end(),
                   this->m_UnlockedTitles.begin(), arrayConversion);

    const auto& unlockedAchievements =
        jv.at("unlocked_achievements").as_array();
    std::transform(unlockedAchievements.begin(), unlockedAchievements.end(),
                   this->m_UnlockedAchievements.begin(), arrayConversion);

    this->m_Signature = jv.at("signature").as_string();

    // TODO: create a new field for this in the db
    // this->m_NumUnreadMessages = jv.at("unread_messages").as_int64();
    this->m_NumUnreadMessages = 0;

    this->m_CampaignFlags = CampaignFlags(jv.at("campaign_flags").as_int64());

    this->m_NetcafeName = jv.at("netcafe_name").as_string();

    this->m_ClanName = jv.at("clan_name").as_string();
    this->m_ClanMark =
        util::FastNarrow<std::uint32_t>(jv.at("clan_mark").as_int64());

    this->m_WorldRank =
        util::FastNarrow<std::uint32_t>(jv.at("world_rank").as_int64());

    this->m_BestGamemode =
        util::FastNarrow<std::uint32_t>(jv.at("best_gamemode").as_int64());
    this->m_BestMap =
        util::FastNarrow<std::uint32_t>(jv.at("best_map").as_int64());

    this->m_SkillHumanCurXp =
        JStrToUint64(jv.at("skill_human_curxp").as_string());
    this->m_SkillHumanMaxXp =
        JStrToUint64(jv.at("skill_human_maxxp").as_string());
    this->m_SkillHumanPoints =
        util::FastNarrow<std::uint8_t>(jv.at("skill_human_points").as_int64());
    this->m_SkillZombieCurXp =
        JStrToUint64(jv.at("skill_zombie_curxp").as_string());
    this->m_SkillZombieMaxXp =
        JStrToUint64(jv.at("skill_zombie_maxxp").as_string());
    this->m_SkillZombiePoints =
        util::FastNarrow<std::uint8_t>(jv.at("skill_zombie_points").as_int64());

    this->m_ForceRelayHost = jv.at("force_relay_host").as_bool();
}
}  // namespace cso2
