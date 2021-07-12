#include "packets/in/room/updatesettings.hpp"

#include "cso2/definitions.hpp"
#include "packets/view.hpp"
#include "room/settings.hpp"

InRoomPacketUpdateSettings::InRoomPacketUpdateSettings(PacketView& packet)
{
    this->Flags = packet.Read<std::uint64_t>();

    if (this->Flags & 0x1)
    {
        this->Settings.SetRoomName(packet.ReadString());
    }

    if (this->Flags & 0x2)
    {
        this->Settings.SetUnk00(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x4)
    {
        auto unk01 = packet.Read<std::uint8_t>();
        auto unk02 = packet.Read<std::uint32_t>();
        auto unk03 = packet.Read<std::uint32_t>();
        this->Settings.SetUnk01(unk01, unk02, unk03);
    }

    if (this->Flags & 0x8)
    {
        this->Settings.SetPassword(packet.ReadString());
    }

    if (this->Flags & 0x10)
    {
        this->Settings.SetUnk10(packet.Read<std::uint16_t>());
    }

    if (this->Flags & 0x20)
    {
        this->Settings.SetForceCamera(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x40)
    {
        this->Settings.SetGameMode(cso2::GameMode(packet.Read<std::uint8_t>()));
    }

    if (this->Flags & 0x80)
    {
        auto mapId = packet.Read<std::uint8_t>();
        auto unk13 = packet.Read<std::uint8_t>();
        this->Settings.SetMapId(mapId);
    }

    if (this->Flags & 0x100)
    {
        this->Settings.SetMaxPlayers(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x200)
    {
        this->Settings.SetWinLimit(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x400)
    {
        this->Settings.SetKillLimit(packet.Read<std::uint16_t>());
    }

    if (this->Flags & 0x800)
    {
        this->Settings.SetUnk17(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x1000)
    {
        this->Settings.SetUnk18(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x2000)
    {
        this->Settings.SetWeaponRestrictions(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x4000)
    {
        this->Settings.SetStatus(RoomStatus(packet.Read<std::uint8_t>()));
    }

    if (this->Flags & 0x8000)
    {
        auto unk21 = packet.Read<std::uint8_t>();
        auto mapCycleType = packet.Read<std::uint8_t>();
        auto unk23 = packet.Read<std::uint8_t>();
        auto unk24 = packet.Read<std::uint8_t>();
        this->Settings.SetMapCycleType(mapCycleType);
    }

    if (this->Flags & 0x10000)
    {
        this->Settings.SetUnk25(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x20000)
    {
        auto numOfMultimaps = packet.Read<std::uint8_t>();

        std::vector<std::uint8_t> multiMaps;
        multiMaps.reserve(numOfMultimaps);

        for (std::size_t i = 0; i < numOfMultimaps; i++)
        {
            multiMaps.push_back(packet.Read<std::uint8_t>());
        }

        this->Settings.SetMultiMaps(std::move(multiMaps));
    }

    if (this->Flags & 0x40000)
    {
        this->Settings.SetTeamBalanceType(
            cso2::TeamBalanceType(packet.Read<std::uint8_t>()));
    }

    if (this->Flags & 0x80000)
    {
        this->Settings.SetUnk29(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x100000)
    {
        this->Settings.SetUnk30(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x200000)
    {
        this->Settings.SetUnk31(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x400000)
    {
        this->Settings.SetUnk32(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x800000)
    {
        this->Settings.SetUnk33(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x1000000)
    {
        bool botsEnabled = packet.Read<std::uint8_t>() == 1;

        std::uint8_t botDifficulty = 0;
        std::uint8_t numCtBots = 0;
        std::uint8_t numTerBots = 0;

        if (botsEnabled == true)
        {
            botDifficulty = packet.Read<std::uint8_t>();
            numCtBots = packet.Read<std::uint8_t>();
            numTerBots = packet.Read<std::uint8_t>();
        }

        this->Settings.SetBotsEnabled(botsEnabled, botDifficulty, numCtBots,
                                      numTerBots);
    }

    if (this->Flags & 0x2000000)
    {
        this->Settings.SetUnk35(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x4000000)
    {
        this->Settings.SetUnk36(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x8000000)
    {
        this->Settings.SetUnk37(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x10000000)
    {
        this->Settings.SetUnk38(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x20000000)
    {
        this->Settings.SetUnk39(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x40000000)
    {
        this->Settings.SetIngame(packet.Read<std::uint8_t>() == 1);
    }

    if (this->Flags & 0x80000000)
    {
        this->Settings.SetStartMoney(packet.Read<std::uint16_t>());
    }

    if (this->Flags & 0x100000000)
    {
        this->Settings.SetChangeTeams(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x200000000)
    {
        this->Settings.SetUnk43(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x400000000)
    {
        this->Settings.SetSourceTvEnabled(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x800000000)
    {
        this->Settings.SetDifficulty(packet.Read<std::uint8_t>());
    }

    if (this->Flags & 0x1000000000)
    {
        this->Settings.SetRespawnTime(packet.Read<std::uint8_t>());
    }
}
