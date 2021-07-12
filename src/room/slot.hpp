#ifndef __ROOM_SLOT_H_
#define __ROOM_SLOT_H_

#include "clientsession_shared.hpp"
#include "cso2/definitions.hpp"

class Slot
{
public:
    Slot(ClientSessionPtr owner, cso2::TeamNum teamNum);

    [[nodiscard]] ClientSessionPtr GetSession() const noexcept
    {
        return this->m_Owner;
    }

    [[nodiscard]] cso2::TeamNum GetTeam() const noexcept
    {
        return this->m_Team;
    }

    [[nodiscard]] cso2::SlotStatus GetStatus() const noexcept
    {
        return this->m_Status;
    }

    [[nodiscard]] bool IsIngame() const noexcept
    {
        return this->m_Status == cso2::SlotStatus::Ingame;
    }

    [[nodiscard]] std::uint32_t GetUserId() const noexcept
    {
        return this->m_CachedUserId;
    }

    [[nodiscard]] std::uint32_t GetKills() const noexcept
    {
        return this->m_Kills;
    }

    [[nodiscard]] std::uint32_t GetDeaths() const noexcept
    {
        return this->m_Deaths;
    }

    [[nodiscard]] std::uint32_t GetAssists() const noexcept
    {
        return this->m_Assists;
    }

    [[nodiscard]] std::uint32_t GetHeadshots() const noexcept
    {
        return this->m_Headshots;
    }

    void SetTeam(cso2::TeamNum newTeam) noexcept { this->m_Team = newTeam; }

    void SetStatus(cso2::SlotStatus newStatus) noexcept
    {
        this->m_Status = newStatus;
    }

    void AddKills(std::uint32_t numKills) noexcept
    {
        this->m_Kills += numKills;
    }

    void AddDeaths(std::uint32_t numDeaths) noexcept
    {
        this->m_Deaths += numDeaths;
    }

    void AddAssists(std::uint32_t numAssists) noexcept
    {
        this->m_Assists += numAssists;
    }

    void AddHeadshots(std::uint32_t numHeadshots) noexcept
    {
        this->m_Headshots += numHeadshots;
    }

    bool operator==(const Slot& other) const noexcept
    {
        return this->m_Owner == other.m_Owner;
    }

private:
    ClientSessionPtr m_Owner;
    cso2::TeamNum m_Team;
    cso2::SlotStatus m_Status;
    std::uint32_t m_CachedUserId;

    std::uint32_t m_Kills;
    std::uint32_t m_Deaths;
    std::uint32_t m_Assists;
    std::uint32_t m_Headshots;
};

#endif  // __ROOM_SLOT_H_
