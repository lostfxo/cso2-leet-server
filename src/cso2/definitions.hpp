#ifndef __CSO2_DEFINITIONS_H_
#define __CSO2_DEFINITIONS_H_

#include <cstdint>
#include <string_view>

using namespace std::string_view_literals;

namespace cso2
{
enum class CampaignFlags : std::uint16_t
{
    Campaign_0 = 1,
    Campaign_1 = 8 << 0,
    Campaign_2 = 8 << 1,
    Campaign_3 = 8 << 2,
    Campaign_4 = 8 << 3,
    Campaign_5 = 8 << 4,
    Campaign_6 = 8 << 5  // unused
};

inline CampaignFlags operator|(CampaignFlags lhs, CampaignFlags rhs)
{
    using T = std::underlying_type_t<CampaignFlags>;
    return static_cast<CampaignFlags>(static_cast<T>(lhs) |
                                      static_cast<T>(rhs));
}

inline bool IsCampaignFlagValid(CampaignFlags flag)
{
    return flag == CampaignFlags::Campaign_0 ||
           flag == CampaignFlags::Campaign_1 ||
           flag == CampaignFlags::Campaign_2 ||
           flag == CampaignFlags::Campaign_3 ||
           flag == CampaignFlags::Campaign_4 ||
           flag == CampaignFlags::Campaign_5 ||
           flag == CampaignFlags::Campaign_6;
}

enum class ChannelTypes
{
    Free = 0,
    Novice = 1,
    NoviceLowKAD = 2,
    Clan = 3,
    BigCity = 4,
    Active = 5
};

enum class ChannelStatuses
{
    Busy = 0,
    Normal = 1
};

enum class CosmeticSlot
{
    CtItem = 0,
    TerItem = 1,
    HeadItem = 2,
    GloveItem = 3,
    BackItem = 4,
    StepsItem = 5,
    CardItem = 6,
    SprayItem = 7,
};

enum class GameMode : std::uint8_t
{
    original = 1,
    teamdeath = 2,
    zombie = 3,
    stealth = 4,
    gunteamdeath = 5,
    tutorial = 6,
    hide = 7,
    pig = 8,
    animationtest_vcd = 9,
    gz_survivor = 10,
    devtest = 11,
    originalmr = 12,
    originalmrdraw = 13,
    casualbomb = 14,
    deathmatch = 15,
    scenario_test = 16,
    gz = 17,
    gz_intro = 18,
    gz_tour = 19,
    gz_pve = 20,
    eventmod01 = 21,
    duel = 22,
    gz_ZB = 23,
    heroes = 24,
    eventmod02 = 25,
    zombiecraft = 26,
    campaign1 = 27,
    campaign2 = 28,
    campaign3 = 29,
    campaign4 = 30,
    campaign5 = 31,
    campaign6 = 32,
    campaign7 = 33,
    campaign8 = 34,
    campaign9 = 35,
    z_scenario = 36,
    zombie_prop = 37,
    ghost = 38,
    tag = 39,
    hide_match = 40,
    hide_ice = 41,
    diy = 42,
    hide_Item = 43,
    zd_boss1 = 44,
    zd_boss2 = 45,
    zd_boss3 = 46,
    practice = 47,
    zombie_commander = 48,
    casualoriginal = 49,
    hide2 = 50,
    gunball = 51,
    zombie_zeta = 53,
    tdm_small = 54,
    de_small = 55,
    gunteamdeath_re = 56,
    endless_wave = 57,
    rankmatch_original = 58,
    rankmatch_teamdeath = 59,
    play_ground = 60,
    madcity = 61,
    hide_origin = 62,
    teamdeath_mutation = 63,
    giant = 64,
    z_scenario_side = 65,
    hide_multi = 66,
    madcity_team = 67,
    rankmatch_stealth = 68
};

enum class LoadoutSlot : std::uint8_t
{
    PrimaryWeapon = 0,
    SecondaryWeapon = 1,
    Melee = 2,
    HeGrenade = 3,
    Smoke = 4,
    Flash = 5,
};

enum class SlotStatus : std::uint8_t
{
    NotReady = 0,
    Ingame = 1,
    Ready = 2
};

enum class TeamBalanceType : std::uint8_t
{
    Disabled = 0,
    Enabled = 1,
    WithBots = 2,
    ByKadRatio = 4
};

enum class TeamNum : std::uint8_t
{
    Unknown = 0,
    Terrorist = 1,
    CounterTerrorist = 2,
    Spectator = 3  // untested
};

inline bool IsInAnyTeam(TeamNum team)
{
    return team == TeamNum::Terrorist || team == TeamNum::CounterTerrorist;
}

enum class UnlockCurrency
{
    Points = 0,
    Mileage = 1
};

struct UnlockItem
{
    std::uint32_t ItemId;
    std::uint32_t Price;
    UnlockCurrency Currency;
};

struct UnlockProgress
{
    std::uint32_t TargetItemId;
    std::uint32_t ParentItemId;
    std::uint32_t Kills;
};
}  // namespace cso2

#endif  // __CSO2_DEFINITIONS_H_
