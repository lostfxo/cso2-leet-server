#ifndef __ROOM_ROOM_H_
#define __ROOM_ROOM_H_

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>

#include <boost/asio/awaitable.hpp>

#include "cso2/shared.hpp"
#include "holepunchserver.hpp"
#include "room/match.hpp"
#include "room/settings.hpp"
#include "room/shared.hpp"
#include "room/slot.hpp"
#include "util/log.hpp"

using boost::asio::awaitable;

class Room : public std::enable_shared_from_this<Room>
{
public:
    using EmptyCallback_t = std::function<void(RoomPtr)>;

    Room(std::uint32_t roomId, NewRoomRequestOptions& options,
         ClientSessionPtr host);

    ~Room();

    [[nodiscard]] std::uint32_t GetId() const noexcept { return this->m_Id; }

    [[nodiscard]] std::uint32_t GetHostUserId() const noexcept;
    [[nodiscard]] std::string_view GetHostPlayerName() const noexcept;
    [[nodiscard]] std::uint8_t GetHostVipLevel() const noexcept;

    void Join(ClientSessionPtr session);
    void RemoveSlotById(std::uint32_t userId);

    [[nodiscard]] inline std::size_t GetFreeSlots() const noexcept
    {
        return std::max<std::size_t>(
            0, this->m_Settings.GetMaxPlayers() - this->m_Slots.size());
    }

    [[nodiscard]] inline bool IsPasswordProtected() const noexcept
    {
        return this->m_Settings.GetPassword().empty() == false;
    }

    [[nodiscard]] inline bool ComparePassword(
        std::string_view pw) const noexcept
    {
        return this->m_Settings.GetPassword() == pw;
    }

    [[nodiscard]] std::size_t GetNumOfReadyPlayers() const noexcept;
    [[nodiscard]] std::size_t GetNumOfReadyHumanPlayers() const noexcept;
    [[nodiscard]] std::size_t GetNumOfHumanCts() const noexcept;
    [[nodiscard]] std::size_t GetNumOfHumanTerrorists() const noexcept;

    [[nodiscard]] bool IsUserReady(std::uint32_t userId) const noexcept;
    [[nodiscard]] bool IsUserIngame(std::uint32_t userId) const noexcept;
    std::pair<bool, cso2::SlotStatus> ToggleUserReadyStatus(
        std::uint32_t userId);

    [[nodiscard]] const auto& GetSlots() const noexcept
    {
        return this->m_Slots;
    }

    [[nodiscard]] SlotPtr FindSlotById(std::uint32_t userId) noexcept;
    [[nodiscard]] const SlotPtr FindSlotById(
        std::uint32_t userId) const noexcept;

    [[nodiscard]] bool IsUserInRoom(std::uint32_t userId) const;

    [[nodiscard]] const RoomSettings& GetSettings() const noexcept
    {
        return this->m_Settings;
    }

    void UpdateSettings(std::uint64_t updateFlags, RoomSettings&& newSettings);

    [[nodiscard]] RoomStatus GetStatus() const noexcept
    {
        return this->m_Settings.GetStatus();
    }

    [[nodiscard]] std::uint8_t GetCountdown() const noexcept;

    [[nodiscard]] bool IsCountingDown() const noexcept
    {
        return this->m_CountingDown;
    }

    void ProgressCountdown(std::uint8_t hostNextNum) noexcept;
    void StopCountdown() noexcept;

    [[nodiscard]] bool CanStartGame() const noexcept;
    void HostGameStart();
    void GuestJoinGame(std::uint32_t guestUserId);
    awaitable<void> EndGame();

    void OnPlayerKilled(cso2::UserId_t attackerId, cso2::UserId_t victimId,
                        bool headshot);
    void OnRoundEnd(cso2::TeamNum winnerTeam);

    void UpdateUserTeam(std::uint32_t userId, cso2::TeamNum newTeam);

    void AddEmptyListener(EmptyCallback_t&& listener);

protected:
    SlotPtr ReserveSlot(ClientSessionPtr session);
    void SetHost(SlotPtr newHost);

    [[nodiscard]] cso2::TeamNum FindDesirableTeamNum() const noexcept;

    void SetStatus(RoomStatus newStatus);
    void ResetIngameUsersReadyStatus();

    void SendJoinRoom(const SlotPtr sourceSlot) const;
    void SendRoomSettings(const SlotPtr sourceSlot) const;
    void SendTeamChangeGlobal(const SlotPtr sourceSlot,
                              cso2::TeamNum newTeamNum);
    void SendRoomStatusTo(const SlotPtr sourceSlot) const;
    void SendUserReadyStatusTo(const SlotPtr receivingSlot,
                               const SlotPtr sourceSlot) const;
    void SendAllUserStatusTo(const SlotPtr userSlot) const;
    void SendHostConnDataTo(const SlotPtr slot) const;
    void SendGuestConnDataToHost(const SlotPtr slot) const;
    void SendGameEndTo(const SlotPtr slot) const;

    void BroadcastNewUser(const SlotPtr newSlot) const;
    void BroadcastUserLeft(uint32_t leavingUserId) const;
    void BroadcastNewSettings() const;
    void BroadcastUserReadyStatus(const SlotPtr userSlot) const;
    void BroadcastNewUserTeam(const SlotPtr sourceSlot,
                              cso2::TeamNum newTeamNum);
    void BroadcastCountdown(bool shouldCountdown);
    void BroadcastAllReadyStatus() const;

    awaitable<void> RewardUsers();

    void TriggerEmptyEvent();

private:
    std::list<SlotPtr> m_Slots;
    SlotPtr m_Host;

    std::uint32_t m_Id;

    RoomSettings m_Settings;
    Match m_Match;

    std::list<EmptyCallback_t> m_EmptyCallbacks;

    std::uint8_t m_Countdown;

    bool m_CountingDown;
};

#endif  // __ROOM_ROOM_H_
