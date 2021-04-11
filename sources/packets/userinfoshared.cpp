#include "packets/userinfoshared.hpp"

#include "cso2/user.hpp"
#include "packets/builder.hpp"

void WriteUserInfoUpdate(DynamicBuffer& res, std::uint32_t flags, cso2::UserPtr user)
{
    res.Write<std::uint32_t>(flags);  // flags

    if (flags & 0x1)
    {
        res.Write<std::uint64_t>(0x2241158f);  // unk00, nexon id?
    }

    if (flags & 0x2)
    {
        res.WriteString(user->GetPlayerName());  // userName
    }

    if (flags & 0x4)
    {
        res.Write<std::uint16_t>(user->GetLevel());  // level
    }

    if (flags & 0x8)
    {
        res.Write<std::uint64_t>(user->GetCurXp());  // curExp
        res.Write<std::uint64_t>(user->GetMaxXp());  // maxExp
        res.Write<std::uint32_t>(0x313);             // unk03
    }

    if (flags & 0x10)
    {
        res.Write<std::uint8_t>(user->GetRank());  // rank
        res.Write<std::uint8_t>(
            user->GetRankFrame());  // rankframe(item id: 9000-9003)
    }

    if (flags & 0x20)
    {
        res.Write<std::uint64_t>(user->GetPoints());  // Points
    }

    if (flags & 0x40)
    {
        res.Write<std::uint32_t>(user->GetNumOfPlayedMatches());  // played game
        res.Write<std::uint32_t>(
            user->GetWins());  // wins (win rate = wins / played game)
        res.Write<std::uint32_t>(user->GetKills());  // kills
        res.Write<std::uint32_t>(
            user->GetHeadshots());  // headshots (hs rate = hs / kills)
        res.Write<std::uint32_t>(user->GetDeaths());         // deaths
        res.Write<std::uint32_t>(user->GetAssists());        // assists
        res.Write<std::uint16_t>(user->GetAccuracy());       // hit rate
        res.Write<std::uint32_t>(user->GetSecondsPlayed());  // played time (s)
        res.Write<std::uint32_t>(0);                         // unk15
        res.Write<std::uint32_t>(50);                        // unk16
        res.Write<std::uint8_t>(0);                          // unk17
        res.Write<std::uint64_t>(0);                         // unk18
        res.Write<std::uint32_t>(0);                         // unk19
        res.Write<std::uint32_t>(0);                         // unk20
        res.Write<std::uint32_t>(0);                         // unk21
        res.Write<std::uint32_t>(0);                         // unk22
        res.Write<std::uint32_t>(0);                         // unk23
        res.Write<std::uint32_t>(0);                         // unk24
        res.Write<std::uint32_t>(0);                         // unk25
    }

    if (flags & 0x80)
    {
        res.WriteString({});                      // unk26
        res.Write<std::uint32_t>(0);                   // unk27
        res.Write<std::uint32_t>(0);                   // unk28
        res.Write<std::uint32_t>(0);                   // unk29
        res.Write<std::uint32_t>(0);                   // unk30
        res.WriteString(user->GetNetcafeName());  // net cafe
    }

    if (flags & 0x100)
    {
        res.Write<std::uint32_t>(user->GetCash());  // Cash
        res.Write<std::uint32_t>(0);                // unk33
    }

    if (flags & 0x200)
    {
        res.Write<std::uint32_t>(0);                    // unk34
        res.WriteString(user->GetClanName());      // clan name
        res.Write<std::uint32_t>(user->GetClanMark());  // clan mark (0-10)
        res.Write<std::uint8_t>(0);                     // unk37

        // array size is always 5
        const std::array<std::uint32_t, 5> unk38 = { 0, 0, 0, 0, 0 };
        for (const auto elem : unk38)
        {
            res.Write<std::uint32_t>(elem);  // unk38
        }

        // array size is always 5
        const std::array<std::uint32_t, 5> unk39 = { 0, 0, 0, 0, 0 };
        for (const auto elem : unk39)
        {
            res.Write<std::uint32_t>(elem);  // unk39
        }
    }

    if (flags & 0x400)
    {
        res.Write<std::uint8_t>(0);  // unk40
    }

    if (flags & 0x800)
    {
        res.Write<std::uint32_t>(user->GetWorldRank());  // rank in world
        res.Write<std::uint32_t>(0);                     // unk42
    }

    if (flags & 0x1000)
    {
        res.Write<std::uint8_t>(0);  // unk43
        res.Write(std::uint16_t(user->GetCampaignFlags()));
        res.Write<std::uint32_t>(0);  // unk45
    }

    if (flags & 0x2000)
    {
        res.Write<std::uint32_t>(user->GetMileage());  // MPoint
        res.Write<std::uint64_t>(0);                   // unk47
    }

    if (flags & 0x4000)
    {
        res.Write<std::uint32_t>(0);  // unk48
    }

    if (flags & 0x8000)
    {
        res.Write<std::uint16_t>(user->GetTitle());  // title
    }

    if (flags & 0x10000)
    {
        res.Write<std::uint16_t>(0);  // unk50
    }

    if (flags & 0x20000)
    {
        // should always be 128 bytes long
        for (const auto title : user->GetUnlockedTitles())
        {
            res.Write<std::uint8_t>(title);  // title list
        }
    }

    if (flags & 0x40000)
    {
        res.WriteString(user->GetSignature());  // personal signature
    }
    if (flags & 0x80000)
    {
        res.Write<std::uint8_t>(user->GetNumOfUnreadMessages());  // unk53
        res.Write<std::uint8_t>(0);                               // unk54
    }
    if (flags & 0x100000)
    {
        res.Write<std::uint32_t>(7);                        // unk55
        res.Write<std::uint32_t>(user->GetBestGamemode());  // best gamemode
        res.Write<std::uint32_t>(user->GetBestMap());       // best map
    }

    if (flags & 0x200000)
    {
        res.Write<std::uint16_t>(0);  // unk58
    }

    if (flags & 0x400000)
    {
        // it must always be 128 bytes long
        for (const auto ach : user->GetUnlockedAchievements())
        {
            res.Write<std::uint8_t>(
                ach);  // achievement unlocked (all 0xFF only 1024 unlocked);
        }
        res.Write<std::uint32_t>(0xa5c8);  // unk60
    }

    if (flags & 0x800000)
    {
        res.Write<std::uint16_t>(user->GetAvatar());
    }

    if (flags & 0x1000000)
    {
        res.Write<std::uint16_t>(0);  // unk62
    }

    if (flags & 0x2000000)
    {
        // it must always be 128 bytes long
        for (const auto avatar : user->GetUnlockedAvatars())
        {
            res.Write<std::uint8_t>(avatar);  // avatar list
        }
    }

    if (flags & 0x4000000)
    {
        res.Write<std::uint8_t>(user->IsVip() ? 1 : 0);  // isVip
        res.Write<std::uint8_t>(user->GetVipLevel());    // vipLevel
        res.Write<std::uint32_t>(user->GetVipXp());      // vipExp
    }

    if (flags & 0x8000000)
    {
        res.Write<std::uint32_t>(0);  // unk67
    }

    if (flags & 0x10000000)
    {
        // skill factory start
        res.Write<std::uint64_t>(user->GetSkillHumanCurXp());   // human exp
        res.Write<std::uint64_t>(user->GetSkillHumanMaxXp());   // human max exp
        res.Write<std::uint8_t>(user->GetSkillHumanPoints());   // human skill point
        res.Write<std::uint64_t>(user->GetSkillZombieCurXp());  // zombie exp
        res.Write<std::uint64_t>(user->GetSkillZombieMaxXp());  // zombie max exp
        res.Write<std::uint8_t>(user->GetSkillZombiePoints());  // zombie skill point
        // skill factory end
        res.Write<std::uint32_t>(0);  // unk74
        res.Write<std::uint32_t>(0);  // unk75
        res.Write<std::uint32_t>(0);  // unk76
        res.Write<std::uint32_t>(0);  // unk77
        res.Write<std::uint32_t>(0);  // unk78
        res.Write<std::uint32_t>(0);  // unk79
    }

    if (flags & 0x20000000)
    {
        res.Write<std::uint32_t>(0);  // unk80
        res.Write<std::uint32_t>(0);  // unk81
    }

    if (flags & 0x40000000)
    {
        res.Write<std::uint8_t>(0);  // unk82
        res.Write<std::uint8_t>(0);  // unk83
        res.Write<std::uint8_t>(0);  // unk84
    }
}
