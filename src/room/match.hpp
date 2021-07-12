#ifndef __ROOM_MATCH_H_
#define __ROOM_MATCH_H_

#include <cstdint>
#include <stdexcept>
#include <string_view>

#include "cso2/definitions.hpp"
#include "util/log.hpp"

using namespace std::string_view_literals;

enum class MatchStatus
{
    InLobby = 0,
    Waiting = 1,
    Started = 2,
    Ended = 3
};

class Match
{
public:
    Match()
        : m_Status(MatchStatus::InLobby), m_CtRoundsWon(0), m_TerRoundsWon(0)
    {
    }

    [[nodiscard]] MatchStatus GetStatus() const noexcept
    {
        return this->m_Status;
    }

    void SetIngame(bool ingame) noexcept
    {
        this->m_Status = ingame ? MatchStatus::Started : MatchStatus::InLobby;
    }

    void Start()
    {
        if (this->m_Status >= MatchStatus::Started)
        {
            throw std::runtime_error("Match cannot be started");
        }

        this->m_Status = MatchStatus::Started;
    }

    [[nodiscard]] cso2::TeamNum GetWinningTeam() const noexcept
    {
        if (this->m_CtRoundsWon > this->m_TerRoundsWon)
        {
            return cso2::TeamNum::CounterTerrorist;
        }
        else if (this->m_CtRoundsWon < this->m_TerRoundsWon)
        {
            return cso2::TeamNum::Terrorist;
        }
        else
        {
            return cso2::TeamNum::Unknown;
        }
    }

    void ScoreTeam(cso2::TeamNum team)
    {
        if (this->m_Status < MatchStatus::Started)
        {
            throw std::runtime_error(
                "Tried to score a round when the match hasn't started");
        }

        if (team == cso2::TeamNum::CounterTerrorist)
        {
            this->m_CtRoundsWon++;
        }
        else if (team == cso2::TeamNum::Terrorist)
        {
            this->m_TerRoundsWon++;
        }
        else
        {
            throw std::runtime_error("Unknown team has scored");
        }
    }

    [[nodiscard]] std::string GetDebugRoundEndMessage(
        cso2::TeamNum winningTeam) const
    {
        std::string_view teamStr;

        switch (winningTeam)
        {
            case cso2::TeamNum::Terrorist:
                teamStr = "terrorists"sv;
                break;
            case cso2::TeamNum::CounterTerrorist:
                teamStr = "counter-terrorists"sv;
                break;
            default:
                teamStr = "unknown team"sv;
        }

        return fmt::format("{} win the round. ct score: {} ter score: {}\n",
                           teamStr, this->m_CtRoundsWon, this->m_TerRoundsWon);
    }

private:
    MatchStatus m_Status;
    std::uint32_t m_CtRoundsWon;
    std::uint32_t m_TerRoundsWon;
};

#endif  // __ROOM_MATCH_H_
