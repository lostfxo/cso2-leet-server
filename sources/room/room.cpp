#include "room/room.hpp"

#include <algorithm>

#include "channel/channel.hpp"
#include "clientsession.hpp"
#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"
#include "cso2/user.hpp"
#include "services/userservice.hpp"

#include "packets/out/host.hpp"
#include "packets/out/room.hpp"
#include "packets/out/udp.hpp"

namespace ranges = std::ranges;

constexpr const std::uint32_t ROOM_COUNTDOWN_SECONDS = 7;

Room::Room(std::uint32_t roomId, NewRoomRequestOptions& options,
           ClientSessionPtr host)
    : m_Id(roomId), m_Settings(options), m_Countdown(ROOM_COUNTDOWN_SECONDS),
      m_CountingDown(false)
{
    this->m_Host = this->ReserveSlot(host);
    this->SendJoinRoom(*this->m_Host);
    this->SendRoomSettings(*this->m_Host);
}

Room::~Room()
{
    Log::Debug("[Room::~Room] freeing room id {}\n", this->GetId());
}

std::uint32_t Room::GetHostUserId() const noexcept
{
    return this->m_Host->GetUserId();
}

void Room::Join(ClientSessionPtr session)
{
    auto newSlot = this->ReserveSlot(session);
    this->SendJoinRoom(*newSlot);
    this->SendRoomSettings(*newSlot);
    this->SendAllUserStatusTo(*newSlot);
    this->BroadcastNewUser(*newSlot);
}

void Room::RemoveSlotById(std::uint32_t userId)
{
    bool hostDeleted = userId == this->GetHostUserId();

    this->m_Slots.remove_if(
        [userId](const Slot& s) { return s.GetUserId() == userId; });

    if (this->m_Slots.empty() == true)
    {
        this->TriggerEmptyEvent();
        return;
    }

    if (hostDeleted == true)
    {
        auto newHost = this->m_Slots.front();
        Log::Debug("[Room::RemoveSlotById] host removed from the room, setting "
                   "user {} as the new host\n",
                   newHost.GetUserId());
        this->SetHost(newHost);
    }
}

[[nodiscard]] std::size_t Room::GetNumOfReadyPlayers() const noexcept
{
    std::size_t numBotPlayers =
        this->m_Settings.GetNumCtBots() + this->m_Settings.GetNumTrBots();

    if (this->m_Settings.GetTeamBalanceType() ==
        cso2::TeamBalanceType::WithBots)
    {
        std::size_t requiredBalanceBots =
            this->GetNumOfHumanCts() + this->GetNumOfHumanTerrorists();
        numBotPlayers = std::max(numBotPlayers, requiredBalanceBots);
    }

    return this->GetNumOfReadyHumanPlayers() + numBotPlayers;
}

[[nodiscard]] std::size_t Room::GetNumOfReadyHumanPlayers() const noexcept
{
    return ranges::count_if(this->m_Slots, [this](const auto& s) {
        return s.GetStatus() == cso2::SlotStatus::Ready || s == *this->m_Host;
    });
}

[[nodiscard]] std::size_t Room::GetNumOfHumanCts() const noexcept
{
    return ranges::count_if(this->m_Slots, [](const auto& s) {
        return s.GetTeam() == cso2::TeamNum::CounterTerrorist;
    });
}

[[nodiscard]] std::size_t Room::GetNumOfHumanTerrorists() const noexcept
{
    return ranges::count_if(this->m_Slots, [](const auto& s) {
        return s.GetTeam() == cso2::TeamNum::Terrorist;
    });
}

[[nodiscard]] bool Room::IsUserReady(std::uint32_t userId) const noexcept
{
    const auto* slot = this->FindSlotById(userId);

    if (slot == nullptr)
    {
        Log::Warning(
            "[Room::IsUserReady] Could not find slot data for user ID {}\n",
            userId);
        return false;
    }

    return slot->GetStatus() == cso2::SlotStatus::Ready;
}

[[nodiscard]] bool Room::IsUserIngame(std::uint32_t userId) const noexcept
{
    const auto* slot = this->FindSlotById(userId);

    if (slot == nullptr)
    {
        Log::Warning(
            "[Room::IsUserIngame] Could not find slot data for user ID {}\n",
            userId);
        return false;
    }

    return slot->GetStatus() == cso2::SlotStatus::Ingame;
}

std::pair<bool, cso2::SlotStatus> Room::ToggleUserReadyStatus(
    std::uint32_t userId)
{
    if (userId == this->m_Host->GetUserId())
    {
        Log::Warning("[Room::ToggleUserReadyStatus] tried to toggle host's "
                     "ready status\n");
        return { false, cso2::SlotStatus::NotReady };
    }

    auto slot = this->FindSlotById(userId);

    if (slot == nullptr)
    {
        Log::Warning("[Room::ToggleUserReadyStatus] could not find user "
                     "{}'s slot'\n",
                     userId);
        return { false, cso2::SlotStatus::NotReady };
    }

    if (slot->IsIngame() == true)
    {
        Log::Warning("[Room::ToggleUserReadyStatus] user {} tried to "
                     "toggle ready status while ingame\n",
                     userId);
        return { false, cso2::SlotStatus::NotReady };
    }

    cso2::SlotStatus newStatus;

    switch (slot->GetStatus())
    {
        case cso2::SlotStatus::NotReady:
            newStatus = cso2::SlotStatus::Ready;
            break;
        case cso2::SlotStatus::Ready:
            newStatus = cso2::SlotStatus::NotReady;
            break;
        default:
            Log::Warning("[Room::ToggleUserReadyStatus] user {} tried to "
                         "toggle with invalid status {}\n",
                         userId, slot->GetStatus());
            return { false, cso2::SlotStatus::NotReady };
    }

    slot->SetStatus(newStatus);
    this->BroadcastUserReadyStatus(*slot);

    return { true, newStatus };
}

[[nodiscard]] Slot* Room::FindSlotById(std::uint32_t userId) noexcept
{
    return std::addressof(*ranges::find_if(
        this->m_Slots, [userId](Slot& s) { return s.GetUserId() == userId; }));
}

[[nodiscard]] const Slot* Room::FindSlotById(
    std::uint32_t userId) const noexcept
{
    return std::addressof(*ranges::find_if(
        this->m_Slots,
        [userId](const Slot& s) { return s.GetUserId() == userId; }));
}

bool Room::IsUserInRoom(std::uint32_t userId) const
{
    return ranges::find_if(this->m_Slots, [userId](const Slot& slot) {
               return slot.GetUserId() == userId;
           }) != this->m_Slots.end();
}

void Room::UpdateSettings(std::uint64_t updateFlags, RoomSettings&& newSettings)
{
    this->m_Settings.Update(updateFlags, std::move(newSettings));
    this->BroadcastNewSettings();
}

[[nodiscard]] std::uint8_t Room::GetCountdown() const noexcept
{
    if (this->m_CountingDown == false)
    {
        Log::Warning("[Room::GetCountdown] Tried to get countdown without "
                     "counting down\n");
        return 0;
    }

    if (this->m_Countdown > ROOM_COUNTDOWN_SECONDS)
    {
        Log::Warning("[Room::GetCountdown] Our countdown is out of bounds with "
                     "value {}\n",
                     this->m_Countdown);
        return 0;
    }

    return this->m_Countdown;
}

void Room::ProgressCountdown(std::uint8_t hostNextNum) noexcept
{
    this->m_CountingDown = true;

    if (this->m_Countdown > ROOM_COUNTDOWN_SECONDS)
    {
        Log::Warning("[Room::ProgressCountdown] Our countdown is out of bounds "
                     "with value {}\n",
                     this->m_Countdown);
        this->m_Countdown = 0;
    }

    this->m_Countdown--;

    this->BroadcastCountdown(true);

    if (this->m_Countdown != hostNextNum)
    {
        Log::Warning("[Room::ProgressCountdown] host countdown does not "
                     "match ours\n");
    }
}

void Room::StopCountdown() noexcept
{
    this->m_CountingDown = false;
    this->m_Countdown = ROOM_COUNTDOWN_SECONDS;
    this->BroadcastCountdown(false);
}

[[nodiscard]] bool Room::CanStartGame() const noexcept
{
    switch (this->m_Settings.GetGameMode())
    {
        case cso2::GameMode::deathmatch:
        case cso2::GameMode::original:
        case cso2::GameMode::originalmr:
        case cso2::GameMode::casualbomb:
        case cso2::GameMode::casualoriginal:
        case cso2::GameMode::eventmod01:
        case cso2::GameMode::eventmod02:
        case cso2::GameMode::diy:
        case cso2::GameMode::campaign1:
        case cso2::GameMode::campaign2:
        case cso2::GameMode::campaign3:
        case cso2::GameMode::campaign4:
        case cso2::GameMode::campaign5:
        case cso2::GameMode::tdm_small:
        case cso2::GameMode::de_small:
        case cso2::GameMode::madcity:
        case cso2::GameMode::madcity_team:
        case cso2::GameMode::gunteamdeath:
        case cso2::GameMode::gunteamdeath_re:
        case cso2::GameMode::stealth:
        case cso2::GameMode::teamdeath:
        case cso2::GameMode::teamdeath_mutation:
            if (this->GetNumOfReadyPlayers() < 2)
            {
                return false;
            }
            break;
        case cso2::GameMode::giant:
        case cso2::GameMode::hide:
        case cso2::GameMode::hide2:
        case cso2::GameMode::hide_match:
        case cso2::GameMode::hide_origin:
        case cso2::GameMode::hide_Item:
        case cso2::GameMode::hide_multi:
        case cso2::GameMode::ghost:
        case cso2::GameMode::pig:
        case cso2::GameMode::tag:
        case cso2::GameMode::zombie:
        case cso2::GameMode::zombiecraft:
        case cso2::GameMode::zombie_commander:
        case cso2::GameMode::zombie_prop:
        case cso2::GameMode::zombie_zeta:
            if (this->GetNumOfReadyHumanPlayers() < 2)
            {
                return false;
            }
            break;
        default:
            break;
    }

    return true;
}

void Room::HostGameStart()
{
    this->StopCountdown();
    this->SetStatus(RoomStatus::Ingame);
    this->m_Host->SetStatus(cso2::SlotStatus::Ingame);

    for (auto& slot : this->m_Slots)
    {
        if (slot == *this->m_Host)
        {
            continue;
        }

        this->SendRoomStatusTo(slot);

        if (slot.GetStatus() == cso2::SlotStatus::Ready)
        {
            slot.SetStatus(cso2::SlotStatus::Ingame);
            this->SendHostConnDataTo(slot);
            this->SendGuestConnDataToHost(slot);
        }
    }

    this->BroadcastAllReadyStatus();

    this->m_Host->GetSession()->Send(
        OutHostPacket::GameStart(this->m_Host->GetUserId()));
}

void Room::GuestJoinGame(std::uint32_t guestUserId)
{
    auto guestSlot = this->FindSlotById(guestUserId);

    if (guestSlot == nullptr)
    {
        Log::Warning(
            "[Room::GuestJoinGame] could not find slot with user id {}\n",
            guestUserId);
        return;
    }

    this->SendRoomStatusTo(*guestSlot);
    guestSlot->SetStatus(cso2::SlotStatus::Ingame);

    this->SendHostConnDataTo(*guestSlot);
    this->SendGuestConnDataToHost(*guestSlot);

    this->BroadcastAllReadyStatus();
}

awaitable<void> Room::EndGame()
{
    co_await this->RewardUsers();

    this->SetStatus(RoomStatus::Waiting);

    for (auto& slot : this->m_Slots)
    {
        this->SendRoomStatusTo(slot);

        if (slot.IsIngame() == true)
        {
            this->SendGameEndTo(slot);
            slot.SetStatus(cso2::SlotStatus::NotReady);
        }
    }

    this->ResetIngameUsersReadyStatus();
    this->BroadcastAllReadyStatus();
}

void Room::OnPlayerKilled(cso2::UserId_t attackerId, cso2::UserId_t victimId,
                          bool headshot)
{
    if (attackerId != cso2::INVALID_USER_ID)
    {
        auto attackerSlot = this->FindSlotById(attackerId);

        if (attackerSlot == nullptr)
        {
            Log::Warning(
                "[Room::OnPlayerKilled] failed to get attacker {}'s slot, "
                "not counting frag\n",
                attackerId);
            return;
        }

        attackerSlot->AddKills(1);

        if (headshot == true)
        {
            attackerSlot->AddHeadshots(1);
        }
    }

    if (victimId != cso2::INVALID_USER_ID)
    {
        auto victimSlot = this->FindSlotById(victimId);

        if (victimSlot == nullptr)
        {
            Log::Warning(
                "[Room::OnPlayerKilled] failed to get victim {}'s slot, "
                "not counting frag\n",
                victimId);
            return;
        }

        victimSlot->AddDeaths(1);
    }
}

void Room::OnRoundEnd(cso2::TeamNum winnerTeam)
{
    try
    {
        this->m_Match.ScoreTeam(winnerTeam);
        Log::Debug("[Room::OnRoundEnd] {}\n",
                   this->m_Match.GetDebugRoundEndMessage(winnerTeam));
    }
    catch (const std::exception& e)
    {
        Log::Warning("[Room::OnRoundEnd] {}\n", e.what());
    }
}

void Room::UpdateUserTeam(std::uint32_t userId, cso2::TeamNum newTeam)
{
    auto slot = this->FindSlotById(userId);

    if (slot == nullptr)
    {
        Log::Warning("[Room::UpdateUserTeam] could not find slot for user {}\n",
                     userId);
        return;
    }

    slot->SetTeam(newTeam);

    if (this->m_Settings.AreBotsEnabled() == true && slot == this->m_Host)
    {
        for (auto& curSlot : this->m_Slots)
        {
            curSlot.SetTeam(newTeam);
        }
    }

    this->BroadcastNewUserTeam(*slot, newTeam);
}

void Room::AddEmptyListener(Room::EmptyCallback_t&& listener)
{
    this->m_EmptyCallbacks.push_back(std::move(listener));
}

Slot* Room::ReserveSlot(ClientSessionPtr session)
{
    return std::addressof(
        this->m_Slots.emplace_back(session, this->FindDesirableTeamNum()));
}

void Room::SetHost(Slot& newHost)
{
    this->m_Host = std::addressof(newHost);

    for (const auto& s : this->m_Slots)
    {
        s.GetSession()->Send(OutRoomPacket::SetHost(newHost.GetUserId()));
    }
}

[[nodiscard]] cso2::TeamNum Room::FindDesirableTeamNum() const noexcept
{
    std::uint32_t trNum = 0;
    std::uint32_t ctNum = 0;

    for (const auto& slot : this->m_Slots)
    {
        auto team = slot.GetTeam();

        if (team == cso2::TeamNum::Terrorist)
        {
            trNum++;
        }
        else if (team == cso2::TeamNum::CounterTerrorist)
        {
            ctNum++;
        }
        else
        {
            Log::Warning("Room::FindDesirableTeamNum: Unknown team number {}\n",
                         team);
        }
    }

    // send new players to the host's team, if bot mode is enabled
    if (this->m_Settings.AreBotsEnabled() == true)
    {
        // make sure there is at least one human player
        if (trNum + ctNum > 0)
        {
            return this->m_Host->GetTeam();
        }
    }

    if (trNum < ctNum)
    {
        return cso2::TeamNum::Terrorist;
    }
    else
    {
        return cso2::TeamNum::CounterTerrorist;
    }
}

void Room::SetStatus(RoomStatus newStatus)
{
    bool newIngame = newStatus == RoomStatus::Ingame;

    this->m_Settings.SetStatus(newStatus);
    this->m_Settings.SetIngame(newIngame);
    this->m_Match.SetIngame(newIngame);
}

void Room::ResetIngameUsersReadyStatus()
{
    for (auto& slot : this->m_Slots)
    {
        slot.SetStatus(cso2::SlotStatus::NotReady);
    }
}

void Room::SendJoinRoom(const Slot& sourceSlot) const
{
    sourceSlot.GetSession()->Send(OutRoomPacket::CreateAndJoin(*this));
}

void Room::SendRoomSettings(const Slot& sourceSlot) const
{
    sourceSlot.GetSession()->Send(
        OutRoomPacket::UpdateSettings(this->m_Settings));
}

void Room::SendTeamChangeGlobal(const Slot& sourceSlot,
                                cso2::TeamNum newTeamNum)
{
    auto sourceUserId = sourceSlot.GetUserId();

    for (const auto& slot : this->m_Slots)
    {
        slot.GetSession()->Send(
            OutRoomPacket::SetUserTeam(sourceUserId, newTeamNum));
    }
}

void Room::SendRoomStatusTo(const Slot& sourceSlot) const
{
    sourceSlot.GetSession()->Send(OutRoomPacket::UpdateStatusSettings(
        this->m_Settings.GetStatus(), this->m_Settings.IsIngame()));
}

void Room::SendUserReadyStatusTo(const Slot& receivingSlot,
                                 const Slot& sourceSlot) const
{
    receivingSlot.GetSession()->Send(OutRoomPacket::SetPlayerReady(
        sourceSlot.GetUserId(), sourceSlot.GetStatus()));
}

void Room::SendAllUserStatusTo(const Slot& userSlot) const
{
    for (const auto& slot : this->m_Slots)
    {
        this->SendUserReadyStatusTo(userSlot, slot);
    }
}

void Room::SendHostConnDataTo(const Slot& slot) const
{
    auto hostUserId = this->m_Host->GetUserId();
    auto hostSession = this->m_Host->GetSession();

    auto session = slot.GetSession();
    session->Send(
        OutUdpPacket::Udp(hostUserId, hostSession->GetExternalAddress(),
                          hostSession->GetExternalServerPort(), true));
    session->Send(OutHostPacket::HostJoin(hostUserId));
}

void Room::SendGuestConnDataToHost(const Slot& slot) const
{
    auto guestSession = slot.GetSession();

    this->m_Host->GetSession()->Send(
        OutUdpPacket::Udp(slot.GetUserId(), guestSession->GetExternalAddress(),
                          guestSession->GetExternalClientPort(), false));
}

void Room::SendGameEndTo(const Slot& slot) const
{
    auto s = slot.GetSession();
    s->Send(OutHostPacket::HostStop());
    s->Send(OutRoomPacket::SetGameResult());
}

void Room::BroadcastNewUser(const Slot& newSlot) const
{
    for (const auto& s : this->m_Slots)
    {
        // don't send the new user message to the new user
        if (s == newSlot)
        {
            continue;
        }

        s.GetSession()->Send(OutRoomPacket::PlayerJoin(newSlot));
        s.GetSession()->Send(OutRoomPacket::SetPlayerReady(
            newSlot.GetUserId(), newSlot.GetStatus()));
    }
}

void Room::BroadcastNewSettings() const
{
    for (auto& s : this->m_Slots)
    {
        s.GetSession()->Send(OutRoomPacket::UpdateSettings(this->m_Settings));
    }
}

void Room::BroadcastUserReadyStatus(const Slot& userSlot) const
{
    for (auto& s : this->m_Slots)
    {
        s.GetSession()->Send(OutRoomPacket::SetPlayerReady(
            userSlot.GetUserId(), userSlot.GetStatus()));
    }
}

void Room::BroadcastNewUserTeam(const Slot& sourceSlot,
                                cso2::TeamNum newTeamNum)
{
    auto sourceUserId = sourceSlot.GetUserId();

    for (auto& s : this->m_Slots)
    {
        s.GetSession()->Send(
            OutRoomPacket::SetUserTeam(sourceUserId, newTeamNum));

        if (this->m_Settings.AreBotsEnabled() == true)
        {
            s.SetTeam(newTeamNum);
            this->SendTeamChangeGlobal(s, newTeamNum);
        }
    }
}

void Room::BroadcastCountdown(bool shouldCountdown)
{
    if (shouldCountdown == true)
    {
        for (const auto& s : this->m_Slots)
        {
            s.GetSession()->Send(
                OutRoomPacket::ProgressCountdown(this->GetCountdown()));
        }
    }
    else
    {
        for (const auto& s : this->m_Slots)
        {
            s.GetSession()->Send(OutRoomPacket::StopCountdown());
        }
    }
}

void Room::BroadcastAllReadyStatus() const
{
    for (const auto& s : this->m_Slots)
    {
        this->SendAllUserStatusTo(s);
    }
}

awaitable<void> Room::RewardUsers()
{
    auto winnerTeam = this->m_Match.GetWinningTeam();

    for (auto& slot : this->m_Slots)
    {
        if (slot.IsIngame() == false)
        {
            continue;
        }

        auto user = slot.GetSession()->GetUser();

        bool wonGame = winnerTeam == slot.GetTeam();

        bool res = co_await g_UserService->SetUserStats(
            slot.GetUserId(),
            { .Kills = user->GetKills() + slot.GetKills(),
              .Deaths = user->GetDeaths() + slot.GetDeaths(),
              .Assists = user->GetAssists() + slot.GetAssists(),
              .Headshots = user->GetHeadshots() + slot.GetHeadshots(),
              .MatchesPlayed = user->GetNumOfPlayedMatches() + 1,
              .Wins = wonGame ? user->GetWins() + 1 : user->GetWins() });

        if (res == false)
        {
            Log::Warning("[Room::RewardUsers] failed to set user {}'s stats\n",
                         slot.GetUserId());
        }
    }
}

void Room::TriggerEmptyEvent()
{
    for (auto& callback : this->m_EmptyCallbacks)
    {
        callback(shared_from_this());
    }
}
