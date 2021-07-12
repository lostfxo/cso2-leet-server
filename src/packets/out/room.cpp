#include "packets/out/room.hpp"

#include <gsl/gsl>

#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "packets/userinfoshared.hpp"
#include "room/room.hpp"
#include "room/settings.hpp"

inline void WritePlayerNetInfo(DynamicBuffer& buf, cso2::TeamNum teamNum,
                               const ClientSessionPtr session)
{
    buf.Write(std::uint8_t(teamNum));
    buf.Write<std::uint8_t>(0);  // unk01
    buf.Write<std::uint8_t>(0);  // unk02
    buf.Write<std::uint32_t>(session->GetExternalAddress(), false);
    buf.Write<std::uint16_t>(session->GetExternalServerPort());
    buf.Write<std::uint16_t>(session->GetExternalClientPort());
    buf.Write<std::uint16_t>(session->GetExternalTvPort());
    buf.Write<std::uint32_t>(session->GetInternalAddress(), false);
    buf.Write<std::uint16_t>(session->GetInternalServerPort());
    buf.Write<std::uint16_t>(session->GetInternalClientPort());
    buf.Write<std::uint16_t>(session->GetInternalTvPort());
}

inline void WriteRoomSlot(DynamicBuffer& buf, const SlotPtr s)
{
    auto curSession = s->GetSession();
    auto curUser = curSession->GetUser();

    buf.Write<std::uint32_t>(curUser->GetId());
    WritePlayerNetInfo(buf, s->GetTeam(), curSession);
    WriteUserInfoUpdate(buf, 0xFFFFFFFF, curUser);
}

inline void WriteRoomCountdown(DynamicBuffer& buf, bool shouldCount,
                               std::uint8_t countdown = 0)
{
    const auto countType =
        shouldCount ? RoomCountdownType::Progress : RoomCountdownType::Stop;

    buf.Write(std::uint8_t(countType));

    if (countType == RoomCountdownType::Progress)
    {
        buf.Write<std::uint8_t>(countdown);
    }
}

inline void WriteRoomSetTeams(DynamicBuffer& buf,
                              const std::span<std::uint32_t> userIds,
                              const std::span<cso2::TeamNum> respectiveTeams)
{
    if (userIds.size() != respectiveTeams.size())
    {
        throw std::length_error("The user IDs and respective teams arrays must "
                                "have the same number of elements");
    }

    buf.Write(gsl::narrow<std::uint8_t>(userIds.size()));

    for (std::size_t i = 0; i < userIds.size(); i++)
    {
        buf.Write<std::uint32_t>(userIds[i]);
        buf.Write(std::uint8_t(respectiveTeams[i]));
    }
}

PacketBuilder OutRoomPacket::CreateAndJoin(const Room& room)
{
    PacketBuilder res(PacketId::Room, 1024);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::CreateAndJoin));

    const auto& settings = room.GetSettings();

    buf.Write<std::uint32_t>(room.GetHostUserId());  // roomHostId

    buf.Write<std::uint8_t>(2);  // unk01
    buf.Write<std::uint8_t>(2);  // unk02

    buf.Write(gsl::narrow<std::uint16_t>(room.GetId()));  // roomId

    buf.Write<std::uint8_t>(5);  // unk04

    // special class start?
    // flags & 0x1
    buf.Write<std::uint64_t>(0xFFFFFFFFFFFFFFFF);  // roomFlags
    buf.WriteString(settings.GetRoomName());       // roomName
    // end of flags & 0x1
    // flags & 0x2
    buf.Write<std::uint8_t>(0);  // unk05
    // end of flags & 0x2
    // flags & 0x4
    buf.Write<std::uint8_t>(0);   // unk06
    buf.Write<std::uint32_t>(0);  // unk07
    buf.Write<std::uint32_t>(0);  // unk08
    // end of flags & 0x4
    // flags & 0x8
    buf.WriteString("");  // unk09
    // end of flags & 0x8
    // flags & 0x10
    buf.Write<std::uint16_t>(0);  // unk10
    // end of flags & 0x10
    // flags & 0x20
    buf.Write<std::uint8_t>(1);  // unk11
    // end of flags & 0x20
    // flags & 0x40
    buf.Write(std::uint8_t(settings.GetGameMode()));  // gameModeId
    // end of flags & 0x40
    // flags & 0x80
    buf.Write<std::uint8_t>(settings.GetMapId());  // mapId
    buf.Write<std::uint8_t>(0);                    // unk13
    // end of flags & 0x80
    // flags & 0x100
    buf.Write<std::uint8_t>(1);  // unk14
    // end of flags & 0x100
    // flags & 0x200
    buf.Write<std::uint8_t>(settings.GetWinLimit());  // winLimit
    // end of flags & 0x200
    // flags & 0x400
    buf.Write<std::uint16_t>(settings.GetKillLimit());  // killLimit
    // end of flags & 0x400
    // flags & 0x800
    buf.Write<std::uint8_t>(1);  // unk17
    // end of flags & 0x800
    // flags & 0x1000
    buf.Write<std::uint8_t>(10);  // unk18
    // end of flags & 0x1000
    // flags & 0x2000
    buf.Write<std::uint8_t>(0);  // unk19
    // end of flags & 0x2000
    // flags & 0x4000
    buf.Write(std::uint8_t(settings.GetStatus()));  // status
    // end of flags & 0x4000
    // flags & 0x8000
    buf.Write<std::uint8_t>(0);  // unk21
    buf.Write<std::uint8_t>(0);  // unk22
    buf.Write<std::uint8_t>(0);  // unk23
    buf.Write<std::uint8_t>(0);  // unk24
    // end of flags & 0x8000
    // flags & 0x10000
    buf.Write<std::uint8_t>(90);  // unk25
    // end of flags & 0x10000
    // flags & 0x20000
    std::array<std::uint8_t, 0> unk27;          // unk27
    buf.Write<std::uint8_t>(unk27.max_size());  // countOfUnk27
    for (const auto num : unk27)
    {
        buf.Write<std::uint8_t>(num);
    }
    // end of flags & 0x20000
    // flags & 0x40000
    buf.Write<std::uint8_t>(1);  // unk28
    // end of flags & 0x40000
    // flags & 0x80000
    buf.Write<std::uint8_t>(0);  // unk29
    // end of flags & 0x80000
    // flags & 0x100000
    buf.Write<std::uint8_t>(0);  // unk30
    // end of flags & 0x100000
    // flags & 0x20000
    buf.Write<std::uint8_t>(1);  // unk31
    // end of flags & 0x200000
    // flags & 0x400000
    buf.Write<std::uint8_t>(1);  // unk32
    // end of flags & 0x400000
    // flags & 0x800000
    buf.Write<std::uint8_t>(0);  // unk33
    // end of flags & 0x800000
    // flags & 0x1000000
    buf.Write<std::uint8_t>(settings.AreBotsEnabled());  // botEnabled
    if (settings.AreBotsEnabled() == true)
    {
        buf.Write<std::uint8_t>(settings.GetBotDifficulty());  // botDifficulty
        buf.Write<std::uint8_t>(settings.GetNumCtBots());      // numCtBots
        buf.Write<std::uint8_t>(settings.GetNumTrBots());      // numTrBots;
    }
    // end of flags & 0x1000000
    // flags & 0x2000000
    buf.Write<std::uint8_t>(0);  // unk35
    // end of flags & 0x2000000
    // flags & 0x4000000
    buf.Write<std::uint8_t>(0);  // unk36
    // end of flags & 0x4000000
    // flags & 0x8000000
    buf.Write<std::uint8_t>(0);  // unk37
    // end of flags & 0x8000000
    // flags & 0x10000000
    buf.Write<std::uint8_t>(0);  // unk38
    // end of flags & 0x10000000
    // flags & 0x20000000
    buf.Write<std::uint8_t>(0);  // unk39
    // end of flags & 0x20000000
    // flags & 0x40000000
    buf.Write<std::uint8_t>(settings.IsIngame());  // isInGame
    // end of flags & 0x40000000
    // flags & 0x80000000
    buf.Write<std::uint16_t>(16000);  // unk41
    // end of flags & 0x80000000
    // flags & 0x100000000
    buf.Write<std::uint8_t>(0);  // unk42
    // end of flags & 0x100000000
    // flags & 0x200000000
    buf.Write<std::uint8_t>(0);  // unk43
    // end of flags & 0x200000000
    // flags & 0x400000000
    buf.Write<std::uint8_t>(0);  // unk44
    // end of flags & 0x400000000
    // flags & 0x800000000
    buf.Write<std::uint8_t>(0);  // mapLevel
    // end of flags & 0x800000000
    // flags & 0x1000000000
    buf.Write<std::uint8_t>(3);  // unk46
    // end of flags & 0x1000000000
    // special class end?

    const auto& slots = room.GetSlots();

    buf.Write(gsl::narrow<std::uint8_t>(slots.size()));  // numOfPlayers

    for (auto s : slots)
    {
        WriteRoomSlot(buf, s);
    }

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::PlayerJoin(const SlotPtr slot)
{
    PacketBuilder res(PacketId::Room, 256);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::PlayerJoin));
    WriteRoomSlot(buf, slot);

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::PlayerLeave(std::uint32_t userId)
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::PlayerLeave));
    buf.Write<std::uint32_t>(userId);

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::SetPlayerReady(std::uint32_t userId,
                                            cso2::SlotStatus status)
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::SetPlayerReady));
    buf.Write<std::uint32_t>(userId);
    buf.Write(std::uint8_t(status));

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::UpdateSettings(const RoomSettings& settings)
{
    PacketBuilder res(PacketId::Room, 256);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::UpdateSettings));

    const auto flags = settings.GetFlags();
    buf.Write<std::uint64_t>(flags);

    if (flags & 0x1)
    {
        buf.WriteString(settings.GetRoomName());
    }
    /*if (flags & 0x2)
    {
        buf.Write<std::uint8_t>(settings.unk00);
    }
    if (flags & 0x4)
    {
        buf.Write<std::uint8_t>(settings.unk01);
        buf.Write<std::uint32_t>(settings.unk02);
        buf.Write<std::uint32_t>(settings.unk03);
    }*/
    if (flags & 0x8)
    {
        buf.WriteString(settings.GetPassword());
    }
    /*if (flags & 0x10)
    {
        buf.Write<std::uint16_t>(settings.unk10);
    }*/
    if (flags & 0x20)
    {
        buf.Write<std::uint8_t>(settings.GetForceCamera());
    }
    if (flags & 0x40)
    {
        buf.Write(std::uint8_t(settings.GetGameMode()));
    }
    if (flags & 0x80)
    {
        buf.Write<std::uint8_t>(settings.GetMapId());
        buf.Write<std::uint8_t>(settings.GetUnk13());
    }
    if (flags & 0x100)
    {
        buf.Write<std::uint8_t>(settings.GetMaxPlayers());
    }
    if (flags & 0x200)
    {
        buf.Write<std::uint8_t>(settings.GetWinLimit());
    }
    if (flags & 0x400)
    {
        buf.Write<std::uint16_t>(settings.GetKillLimit());
    }
    /*if (flags & 0x800)
    {
        buf.Write<std::uint8_t>(settings.unk17);
    }
    if (flags & 0x1000)
    {
        buf.Write<std::uint8_t>(settings.unk18);
    }*/
    if (flags & 0x2000)
    {
        buf.Write<std::uint8_t>(settings.GetWeaponRestrictions());
    }
    if (flags & 0x4000)
    {
        buf.Write(std::uint8_t(settings.GetStatus()));
    }
    if (flags & 0x8000)
    {
        buf.Write<std::uint8_t>(settings.GetUnk21());
        buf.Write<std::uint8_t>(settings.GetMapCycleType());
        buf.Write<std::uint8_t>(settings.GetUnk23());
        buf.Write<std::uint8_t>(settings.GetUnk24());
    }
    /*if (flags & 0x10000)
    {
        buf.Write<std::uint8_t>(settings.unk25);
    }*/
    if (flags & 0x20000)
    {
        const auto& multiMaps = settings.GetMultiMaps();

        buf.Write(gsl::narrow<std::uint8_t>(multiMaps.size()));
        for (const auto map : multiMaps)
        {
            buf.Write<std::uint8_t>(map);
        }
    }
    if (flags & 0x40000)
    {
        buf.Write(std::uint8_t(settings.GetTeamBalanceType()));
    }
    /*if (flags & 0x80000)
    {
        buf.Write<std::uint8_t>(settings.unk29);
    }
    if (flags & 0x100000)
    {
        buf.Write<std::uint8_t>(settings.unk30);
    }
    if (flags & 0x200000)
    {
        buf.Write<std::uint8_t>(settings.unk31);
    }
    if (flags & 0x400000)
    {
        buf.Write<std::uint8_t>(settings.unk32);
    }
    if (flags & 0x800000)
    {
        buf.Write<std::uint8_t>(settings.unk33);
    }*/
    if (flags & 0x1000000)
    {
        buf.Write<std::uint8_t>(settings.AreBotsEnabled());

        if (settings.AreBotsEnabled() == true)
        {
            buf.Write<std::uint8_t>(settings.GetBotDifficulty());
            buf.Write<std::uint8_t>(settings.GetNumCtBots());
            buf.Write<std::uint8_t>(settings.GetNumTrBots());
        }
    }

    /*if (flags & 0x2000000)
    {
        buf.Write<std::uint8_t>(settings.unk35);
    }

    if (flags & 0x4000000)
    {
        buf.Write<std::uint8_t>(settings.unk36);
    }

    if (flags & 0x8000000)
    {
        buf.Write<std::uint8_t>(settings.unk37);
    }

    if (flags & 0x10000000)
    {
        buf.Write<std::uint8_t>(settings.unk38);
    }

    if (flags & 0x20000000)
    {
        buf.Write<std::uint8_t>(settings.unk39);
    }*/

    if (flags & 0x40000000)
    {
        buf.Write<std::uint8_t>(settings.IsIngame());
    }

    if (flags & 0x80000000)
    {
        buf.Write<std::uint16_t>(settings.GetStartMoney());
    }

    if (flags & 0x100000000)
    {
        buf.Write<std::uint8_t>(settings.GetChangeTeams());
    }

    /*if (flags & 0x200000000)
    {
        buf.Write<std::uint8_t>(settings.unk43);
    }*/

    if (flags & 0x400000000)
    {
        buf.Write<std::uint8_t>(settings.GetSourceTvEnabled());
    }

    if (flags & 0x800000000)
    {
        buf.Write<std::uint8_t>(settings.GetDifficulty());
    }

    if (flags & 0x1000000000)
    {
        buf.Write<std::uint8_t>(settings.GetRespawnTime());
    }

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::UpdateStatusSettings(RoomStatus status,
                                                  bool ingame)
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::UpdateSettings));

    constexpr const auto flags = 0x40000000 | 0x4000;
    buf.Write<std::uint64_t>(flags);

    // flag 0x4000
    buf.Write(std::uint8_t(status));

    // flag 0x40000000
    buf.Write<std::uint8_t>(ingame);

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::SetHost(std::uint32_t userId)
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::SetHost));
    buf.Write<std::uint32_t>(userId);
    buf.Write<std::uint8_t>(0);  // unused

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::SetGameResult()
{
    PacketBuilder res(PacketId::Room, 64);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::SetGameResult));

    buf.Write<std::uint8_t>(0);   // unk00
    buf.Write<std::uint8_t>(0);   // unk01
    buf.Write<std::uint8_t>(0);   // unk02
    buf.Write<std::uint64_t>(0);  // unk03
    buf.Write<std::uint64_t>(0);  // unk04
    buf.Write<std::uint8_t>(0);   // unk05
    buf.WriteString("");          // unk06
    buf.WriteString("");          // unk07
    buf.Write<std::uint8_t>(0);   // unk08
    buf.Write<std::uint8_t>(0);   // unk09

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::SetUserTeam(std::uint32_t userId,
                                         cso2::TeamNum team)
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::SetUserTeam));
    WriteRoomSetTeams(buf, { &userId, 1 }, { &team, 1 });

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::ProgressCountdown(std::uint8_t countdown)
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::Countdown));
    WriteRoomCountdown(buf, true, countdown);

    res.Finish();

    return res;
}

PacketBuilder OutRoomPacket::StopCountdown()
{
    PacketBuilder res(PacketId::Room, 16);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutRoomPacketType::Countdown));
    WriteRoomCountdown(buf, false);

    res.Finish();

    return res;
}
