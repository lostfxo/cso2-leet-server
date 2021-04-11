#include "room/settings.hpp"

RoomSettings::RoomSettings() : m_Flags(0) {}

RoomSettings::RoomSettings(NewRoomRequestOptions& options) : m_Flags(0)
{
    this->SetRoomName(!options.RoomName.empty() ? std::move(options.RoomName) :
                                                  "New Room");
    this->SetPassword(std::move(options.Password));
    this->SetGameMode(options.GameModeId);
    this->SetMapId(options.MapId);
    this->SetWinLimit(options.WinLimit);
    this->SetKillLimit(options.KillLimit);

    this->SetStartMoney(16000);
    this->SetForceCamera(1);
    this->SetChangeTeams(0);
    this->SetBotsEnabled(false);
    this->SetDifficulty(0);
    this->SetMaxPlayers(16);
    this->SetRespawnTime(3);
    this->SetTeamBalanceType(cso2::TeamBalanceType::Disabled);
    this->SetWeaponRestrictions(0);
    this->SetSourceTvEnabled(0);
    this->SetMapCycleType(1);

    this->SetStatus(RoomStatus::Waiting);
    this->SetIngame(false);
}

void RoomSettings::Update(std::uint64_t updateFlags, RoomSettings&& other)
{
    if (updateFlags & 0x1)
    {
        this->m_RoomName = std::move(other.m_RoomName);
    }
    if (updateFlags & 0x2)
    {
        this->unk00 = other.unk00;
    }
    if (updateFlags & 0x4)
    {
        this->unk01 = other.unk01;
        this->unk02 = other.unk02;
        this->unk03 = other.unk03;
    }
    if (updateFlags & 0x8)
    {
        this->m_Password = std::move(other.m_Password);
    }
    if (updateFlags & 0x10)
    {
        this->unk10 = other.unk10;
    }
    if (updateFlags & 0x20)
    {
        this->m_ForceCamera = other.m_ForceCamera;
    }
    if (updateFlags & 0x40)
    {
        this->m_GameMode = other.m_GameMode;
    }
    if (updateFlags & 0x80)
    {
        this->m_MapId = other.m_MapId;
        this->unk13 = other.unk13;
    }
    if (updateFlags & 0x100)
    {
        this->m_MaxPlayers = other.m_MaxPlayers;
    }
    if (updateFlags & 0x200)
    {
        this->m_WinLimit = other.m_WinLimit;
    }
    if (updateFlags & 0x400)
    {
        this->m_KillLimit = other.m_KillLimit;
    }
    if (updateFlags & 0x800)
    {
        this->unk17 = other.unk17;
    }
    if (updateFlags & 0x1000)
    {
        this->unk18 = other.unk18;
    }
    if (updateFlags & 0x2000)
    {
        this->m_WeaponRestrictions = other.m_WeaponRestrictions;
    }
    if (updateFlags & 0x4000)
    {
        this->m_Status = other.m_Status;
    }
    if (updateFlags & 0x8000)
    {
        this->unk21 = other.unk21;
        this->m_MapCycleType = other.m_MapCycleType;
        this->unk23 = other.unk23;
        this->unk24 = other.unk24;
    }
    if (updateFlags & 0x10000)
    {
        this->unk25 = other.unk25;
    }
    if (updateFlags & 0x20000)
    {
        this->m_MultiMaps = std::move(other.m_MultiMaps);
    }
    if (updateFlags & 0x40000)
    {
        this->m_TeamBalanceType = other.m_TeamBalanceType;
    }
    if (updateFlags & 0x80000)
    {
        this->unk29 = other.unk29;
    }
    if (updateFlags & 0x100000)
    {
        this->unk30 = other.unk30;
    }
    if (updateFlags & 0x200000)
    {
        this->unk31 = other.unk31;
    }
    if (updateFlags & 0x400000)
    {
        this->unk32 = other.unk32;
    }
    if (updateFlags & 0x800000)
    {
        this->unk33 = other.unk33;
    }
    if (updateFlags & 0x1000000)
    {
        this->m_BotsEnabled = other.m_BotsEnabled;

        if (this->m_BotsEnabled == true)
        {
            this->m_BotDifficulty = other.m_BotDifficulty;
            this->m_NumCtBots = other.m_NumCtBots;
            this->m_NumTrBots = other.m_NumTrBots;
        }
        else
        {
            this->m_BotDifficulty = 0;
            this->m_NumCtBots = 0;
            this->m_NumTrBots = 0;
        }
    }
    if (updateFlags & 0x2000000)
    {
        this->unk35 = other.unk35;
    }
    if (updateFlags & 0x4000000)
    {
        this->unk36 = other.unk36;
    }
    if (updateFlags & 0x8000000)
    {
        this->unk37 = other.unk37;
    }
    if (updateFlags & 0x10000000)
    {
        this->unk38 = other.unk38;
    }
    if (updateFlags & 0x20000000)
    {
        this->unk39 = other.unk39;
    }
    if (updateFlags & 0x40000000)
    {
        this->m_Ingame = other.m_Ingame;
    }
    if (updateFlags & 0x80000000)
    {
        this->m_StartMoney = other.m_StartMoney;
    }
    if (updateFlags & 0x100000000)
    {
        this->m_ChangeTeams = other.m_ChangeTeams;
    }
    if (updateFlags & 0x200000000)
    {
        this->unk43 = other.unk43;
    }
    if (updateFlags & 0x400000000)
    {
        this->m_SourceTvEnabled = other.m_SourceTvEnabled;
    }
    if (updateFlags & 0x800000000)
    {
        this->m_Difficulty = other.m_Difficulty;
    }
    if (updateFlags & 0x1000000000)
    {
        this->m_RespawnTime = other.m_RespawnTime;
    }

    this->m_Flags |= updateFlags;
}

void RoomSettings::SetRoomName(std::string&& newName)
{
    this->m_RoomName = std::move(newName);
    this->m_Flags |= 0x1;
}

void RoomSettings::SetPassword(std::string&& newPassword)
{
    this->m_Password = std::move(newPassword);
    this->m_Flags |= 0x8;
}

void RoomSettings::SetForceCamera(std::uint8_t newCamera)
{
    this->m_ForceCamera = newCamera;
    this->m_Flags |= 0x20;
}

void RoomSettings::SetGameMode(cso2::GameMode newGameMode)
{
    this->m_GameMode = newGameMode;
    this->m_Flags |= 0x40;
}

void RoomSettings::SetMapId(std::uint8_t newMap)
{
    this->m_MapId = newMap;
    this->m_Flags |= 0x80;
    this->unk13 = 0;
}

void RoomSettings::SetMaxPlayers(std::uint8_t newMaxPlayers)
{
    this->m_MaxPlayers = newMaxPlayers;

    // reset bot number to 4 for each team when max players is changed
    // and the old max players value is bigger than the new one
    // this stops overflowing the max player number (where bots are included)
    if (this->AreBotsEnabled() == true && this->m_MaxPlayers > newMaxPlayers)
    {
        this->m_NumCtBots = this->m_NumTrBots = 4;
    }

    this->m_Flags |= 0x100;
}

void RoomSettings::SetWinLimit(std::uint8_t newLimit)
{
    this->m_WinLimit = newLimit;
    this->m_Flags |= 0x200;
}

void RoomSettings::SetKillLimit(std::uint16_t newLimit)
{
    this->m_KillLimit = newLimit;
    this->m_Flags |= 0x400;
}

void RoomSettings::SetWeaponRestrictions(std::uint8_t newRestrictions)
{
    this->m_WeaponRestrictions = newRestrictions;
    this->m_Flags |= 0x2000;
}

void RoomSettings::SetStatus(RoomStatus newStatus)
{
    this->m_Status = newStatus;
    this->m_Flags |= 0x4000;
}

void RoomSettings::SetMapCycleType(std::uint8_t newType)
{
    this->unk21 = 0;
    this->m_MapCycleType = newType;
    this->unk23 = 0;
    this->unk24 = 0;
    this->m_Flags |= 0x8000;
}

void RoomSettings::SetMultiMaps(std::vector<std::uint8_t>&& newMaps)
{
    this->m_MultiMaps = std::move(newMaps);
    this->m_Flags |= 0x20000;
}

void RoomSettings::SetTeamBalanceType(cso2::TeamBalanceType newType)
{
    this->m_TeamBalanceType = newType;
    this->m_Flags |= 0x40000;
}

void RoomSettings::SetBotsEnabled(bool enabled,
                                  std::uint8_t botDifficulty /*= 0*/,
                                  std::uint8_t numCtBots /*= 0*/,
                                  std::uint8_t numTerBots /*= 0*/)
{
    this->m_BotsEnabled = enabled;
    this->m_BotDifficulty = botDifficulty;
    this->m_NumCtBots = numCtBots;
    this->m_NumTrBots = numTerBots;
    this->m_Flags |= 0x1000000;
}

void RoomSettings::SetIngame(bool newIngame)
{
    this->m_Ingame = newIngame;
    this->m_Flags |= 0x40000000;
}

void RoomSettings::SetStartMoney(std::uint16_t newStartMoney)
{
    this->m_Ingame = newStartMoney;
    this->m_Flags |= 0x80000000;
}

void RoomSettings::SetChangeTeams(std::uint8_t newChange)
{
    this->m_ChangeTeams = newChange;
    this->m_Flags |= 0x100000000;
}

void RoomSettings::SetSourceTvEnabled(std::uint8_t newEnabled)
{
    this->m_SourceTvEnabled = newEnabled;
    this->m_Flags |= 0x400000000;
}

void RoomSettings::SetDifficulty(std::uint8_t newDifficulty)
{
    this->m_Difficulty = newDifficulty;
    this->m_Flags |= 0x800000000;
}

void RoomSettings::SetRespawnTime(std::uint8_t newTime)
{
    this->m_RespawnTime = newTime;
    this->m_Flags |= 0x1000000000;
}

void RoomSettings::SetUnk00(std::uint8_t newVal)
{
    this->unk00 = newVal;
    this->m_Flags |= 0x2;
}

void RoomSettings::SetUnk01(std::uint8_t newUnk01, std::uint32_t newUnk02,
                            std::uint32_t newUnk03)
{
    this->unk01 = newUnk01;
    this->unk02 = newUnk02;
    this->unk03 = newUnk03;
    this->m_Flags |= 0x4;
}

void RoomSettings::SetUnk10(std::uint16_t newVal)
{
    this->unk10 = newVal;
    this->m_Flags |= 0x10;
}

void RoomSettings::SetUnk17(std::uint8_t newVal)
{
    this->unk17 = newVal;
    this->m_Flags |= 0x800;
}

void RoomSettings::SetUnk18(std::uint8_t newVal)
{
    this->unk18 = newVal;
    this->m_Flags |= 0x1000;
}

void RoomSettings::SetUnk25(std::uint8_t newVal)
{
    this->unk25 = newVal;
    this->m_Flags |= 0x10000;
}

void RoomSettings::SetUnk29(std::uint8_t newVal)
{
    this->unk29 = newVal;
    this->m_Flags |= 0x80000;
}

void RoomSettings::SetUnk30(std::uint8_t newVal)
{
    this->unk30 = newVal;
    this->m_Flags |= 0x100000;
}

void RoomSettings::SetUnk31(std::uint8_t newVal)
{
    this->unk31 = newVal;
    this->m_Flags |= 0x200000;
}

void RoomSettings::SetUnk32(std::uint8_t newVal)
{
    this->unk32 = newVal;
    this->m_Flags |= 0x400000;
}

void RoomSettings::SetUnk33(std::uint8_t newVal)
{
    this->unk33 = newVal;
    this->m_Flags |= 0x800000;
}

void RoomSettings::SetUnk35(std::uint8_t newVal)
{
    this->unk35 = newVal;
    this->m_Flags |= 0x2000000;
}

void RoomSettings::SetUnk36(std::uint8_t newVal)
{
    this->unk36 = newVal;
    this->m_Flags |= 0x4000000;
}

void RoomSettings::SetUnk37(std::uint8_t newVal)
{
    this->unk37 = newVal;
    this->m_Flags |= 0x8000000;
}

void RoomSettings::SetUnk38(std::uint8_t newVal)
{
    this->unk38 = newVal;
    this->m_Flags |= 0x10000000;
}

void RoomSettings::SetUnk39(std::uint8_t newVal)
{
    this->unk39 = newVal;
    this->m_Flags |= 0x20000000;
}

void RoomSettings::SetUnk43(std::uint8_t newVal)
{
    this->unk43 = newVal;
    this->m_Flags |= 0x200000000;
}
