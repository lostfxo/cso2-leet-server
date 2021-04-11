#ifndef __CSO2_TAKEDAMAGEINFO_H_
#define __CSO2_TAKEDAMAGEINFO_H_

#include <vector>
#include "cso2/definitions.hpp"
#include "cso2/vector.hpp"

namespace cso2
{
enum class KillFlags : std::uint32_t
{
    KilledByHeadshot = 0x1,
    KilledThroughWall = 0x2,
    KilledByJumpshot = 0x4,
    KilledByLongshot = 0x8,
    KilledByKnife = 0x400,
    KilledByFalling = 0x1000,
    KilledWithSomeoneElseWeapon = 0x2000,
    KilledWithOpposingTeamWeapon = 0x4000,
    KilledByScopedSniper = 0x8000,
    KilledByNoscopedSniper = 0x10000,
    KilledWithLastBullet = 0x20000,
    KilledByGrenade = 0x40000,
    KilledByProp = 0x80000,
    KilledBySniper = 0x100000
};

inline bool operator&(KillFlags lhs, KillFlags rhs)
{
    using T = std::underlying_type_t<KillFlags>;
    return static_cast<T>(lhs) & static_cast<T>(rhs);
}

enum class ClientType : std::uint8_t
{
    Human = 0,
    NPC = 1,
    FakeClient = 2
};

enum class CharacterType : std::uint8_t
{
    CSPlayer = 0,
    Pig = 40,
    Zombie = 50,
    ZombieMaster = 51,  // the host zombie
    Sentrygun = 60
};

struct TDIPlayerInfo
{
    // members are memory aligned
    std::uint32_t UserId;
    std::uint32_t WeaponId;
    std::uint32_t CharacterClass;
    TeamNum Team;
    ClientType Type;
    CharacterType Character;
};

struct TDIUnknownInfo
{
    TDIUnknownInfo(std::uint8_t _unk01 = 0, std::uint32_t _unk02 = 0, std::uint32_t _unk03 = 0,
                   std::uint8_t _unk04 = 0)
        : unk02(_unk02), unk03(_unk03), unk01(_unk01), unk04(_unk04)
    {
    }

    // members are memory aligned
    std::uint32_t unk02;
    std::uint32_t unk03;
    std::uint8_t unk01;
    std::uint8_t unk04;
};

struct TakeDamageInfo
{
    KillFlags Flags;
    TDIPlayerInfo Attacker;
    std::uint32_t someVictimSpecialFlags;
    TDIPlayerInfo Victim;
    IntVector AttackerPos;
    IntVector VictimPos;
    std::vector<TDIUnknownInfo> unkArray;
};

}  // namespace cso2

#endif  // __CSO2_TAKEDAMAGEINFO_H_
