#ifndef __ROOM_SETTINGS_H_
#define __ROOM_SETTINGS_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "cso2/definitions.hpp"
#include "room/shared.hpp"

class RoomSettings
{
public:
    RoomSettings();
    RoomSettings(NewRoomRequestOptions& options);

    void Update(std::uint64_t updateFlags, RoomSettings&& other);

    [[nodiscard]] inline std::uint64_t GetFlags() const noexcept
    {
        return this->m_Flags;
    }

    [[nodiscard]] inline std::string_view GetRoomName() const noexcept
    {
        return this->m_RoomName;
    }

    [[nodiscard]] inline std::string_view GetPassword() const noexcept
    {
        return this->m_Password;
    }

    [[nodiscard]] inline std::uint8_t GetMaxPlayers() const noexcept
    {
        return this->m_MaxPlayers;
    }

    [[nodiscard]] inline cso2::GameMode GetGameMode() const noexcept
    {
        return this->m_GameMode;
    }

    [[nodiscard]] inline std::uint8_t GetMapId() const noexcept
    {
        return this->m_MapId;
    }

    [[nodiscard]] inline std::uint8_t GetWinLimit() const noexcept
    {
        return this->m_WinLimit;
    }

    [[nodiscard]] inline std::uint16_t GetKillLimit() const noexcept
    {
        return this->m_KillLimit;
    }

    [[nodiscard]] inline std::uint16_t GetStartMoney() const noexcept
    {
        return this->m_StartMoney;
    }

    [[nodiscard]] inline std::uint8_t GetForceCamera() const noexcept
    {
        return this->m_ForceCamera;
    }

    [[nodiscard]] inline std::uint8_t GetNextMapEnabled() const noexcept
    {
        return this->m_NextMapEnabled;
    }

    [[nodiscard]] inline std::uint8_t GetChangeTeams() const noexcept
    {
        return this->m_ChangeTeams;
    }

    [[nodiscard]] inline std::uint8_t GetDifficulty() const noexcept
    {
        return this->m_Difficulty;
    }

    [[nodiscard]] inline std::uint8_t GetRespawnTime() const noexcept
    {
        return this->m_RespawnTime;
    }

    [[nodiscard]] inline cso2::TeamBalanceType GetTeamBalanceType()
        const noexcept
    {
        return this->m_TeamBalanceType;
    }

    [[nodiscard]] inline std::uint8_t GetWeaponRestrictions() const noexcept
    {
        return this->m_WeaponRestrictions;
    }

    [[nodiscard]] inline RoomStatus GetStatus() const noexcept
    {
        return this->m_Status;
    }

    [[nodiscard]] inline std::uint8_t GetSourceTvEnabled() const noexcept
    {
        return this->m_SourceTvEnabled;
    }

    [[nodiscard]] inline std::uint8_t GetMapCycleType() const noexcept
    {
        return this->m_MapCycleType;
    }

    [[nodiscard]] inline const std::vector<std::uint8_t>& GetMultiMaps()
        const noexcept
    {
        return this->m_MultiMaps;
    }

    [[nodiscard]] inline bool AreBotsEnabled() const noexcept
    {
        return this->m_BotsEnabled;
    }

    [[nodiscard]] inline std::uint8_t GetBotDifficulty() const noexcept
    {
        return this->m_BotDifficulty;
    }

    [[nodiscard]] inline std::uint8_t GetNumCtBots() const noexcept
    {
        return this->m_NumCtBots;
    }

    [[nodiscard]] inline std::uint8_t GetNumTrBots() const noexcept
    {
        return this->m_NumTrBots;
    }

    [[nodiscard]] inline bool IsIngame() const noexcept
    {
        return this->m_Ingame;
    }

    [[nodiscard]] inline std::uint8_t GetUnk13() const noexcept
    {
        return this->unk13;
    }

    [[nodiscard]] inline std::uint8_t GetUnk21() const noexcept
    {
        return this->unk21;
    }

    [[nodiscard]] inline std::uint8_t GetUnk23() const noexcept
    {
        return this->unk23;
    }

    [[nodiscard]] inline std::uint8_t GetUnk24() const noexcept
    {
        return this->unk24;
    }

    void SetRoomName(std::string&& newName);
    void SetPassword(std::string&& newPassword);
    void SetForceCamera(std::uint8_t newCamera);
    void SetGameMode(cso2::GameMode newGameMode);
    void SetMapId(std::uint8_t newMap);
    void SetMaxPlayers(std::uint8_t newMaxPlayers);
    void SetWinLimit(std::uint8_t newLimit);
    void SetKillLimit(std::uint16_t newLimit);
    void SetWeaponRestrictions(std::uint8_t newTime);
    void SetStatus(RoomStatus newStatus);
    void SetMapCycleType(std::uint8_t newType);
    void SetMultiMaps(std::vector<std::uint8_t>&& newMaps);
    void SetTeamBalanceType(cso2::TeamBalanceType newType);
    void SetBotsEnabled(bool enabled, std::uint8_t botDifficulty = 0,
                        std::uint8_t numCtBots = 0,
                        std::uint8_t numTerBots = 0);
    void SetIngame(bool ingame);
    void SetStartMoney(std::uint16_t newStartMoney);
    void SetChangeTeams(std::uint8_t newChange);
    void SetSourceTvEnabled(std::uint8_t newEnabled);
    void SetDifficulty(std::uint8_t newDifficulty);
    void SetRespawnTime(std::uint8_t newTime);

    void SetUnk00(std::uint8_t newVal);
    void SetUnk01(std::uint8_t newUnk01, std::uint32_t newUnk02,
                  std::uint32_t newUnk03);
    void SetUnk10(std::uint16_t newVal);
    void SetUnk17(std::uint8_t newVal);
    void SetUnk18(std::uint8_t newVal);
    void SetUnk25(std::uint8_t newVal);
    void SetUnk29(std::uint8_t newVal);
    void SetUnk30(std::uint8_t newVal);
    void SetUnk31(std::uint8_t newVal);
    void SetUnk32(std::uint8_t newVal);
    void SetUnk33(std::uint8_t newVal);
    void SetUnk35(std::uint8_t newVal);
    void SetUnk36(std::uint8_t newVal);
    void SetUnk37(std::uint8_t newVal);
    void SetUnk38(std::uint8_t newVal);
    void SetUnk39(std::uint8_t newVal);
    void SetUnk43(std::uint8_t newVal);

private:
    std::uint64_t m_Flags;

    std::string m_RoomName;
    std::string m_Password;
    std::uint8_t m_MaxPlayers;
    cso2::GameMode m_GameMode;
    std::uint8_t m_MapId;
    std::uint8_t m_WinLimit;
    std::uint16_t m_KillLimit;
    std::uint16_t m_StartMoney;
    std::uint8_t m_ForceCamera;
    std::uint8_t m_NextMapEnabled;
    std::uint8_t m_ChangeTeams;
    std::uint8_t m_Difficulty;
    std::uint8_t m_RespawnTime;
    cso2::TeamBalanceType m_TeamBalanceType;
    std::uint8_t m_WeaponRestrictions;
    RoomStatus m_Status;
    std::uint8_t m_SourceTvEnabled;
    std::uint8_t m_MapCycleType;
    std::vector<std::uint8_t> m_MultiMaps;

    bool m_BotsEnabled;
    std::uint8_t m_BotDifficulty;
    std::uint8_t m_NumCtBots;
    std::uint8_t m_NumTrBots;
    bool m_Ingame;

    std::uint8_t unk00;
    std::uint8_t unk01;
    std::uint32_t unk02;
    std::uint32_t unk03;
    std::uint16_t unk10;
    std::uint8_t unk13;
    std::uint8_t unk17;
    std::uint8_t unk18;
    std::uint8_t unk21;
    std::uint8_t unk23;
    std::uint8_t unk24;
    std::uint8_t unk25;
    std::uint8_t unk29;
    std::uint8_t unk30;
    std::uint8_t unk31;
    std::uint8_t unk32;
    std::uint8_t unk33;
    std::uint8_t unk35;
    std::uint8_t unk36;
    std::uint8_t unk37;
    std::uint8_t unk38;
    std::uint8_t unk39;
    std::uint8_t unk43;
};

#endif  // __ROOM_SETTINGS_H_
