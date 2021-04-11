#include "handlers.hpp"

#include "channel/channel.hpp"
#include "clientsession.hpp"
#include "cso2/definitions.hpp"
#include "cso2/user.hpp"
#include "packets/view.hpp"
#include "room/room.hpp"
#include "system/dialogbox.hpp"
#include "system/l10n.hpp"
#include "util/log.hpp"

#include "packets/in/room.hpp"
#include "packets/in/room/countdown.hpp"
#include "packets/in/room/joinrequest.hpp"
#include "packets/in/room/newroom.hpp"
#include "packets/in/room/setteam.hpp"
#include "packets/in/room/updatesettings.hpp"
#include "packets/out/host.hpp"

void OnNewRoomRequest(PacketView& packet, ClientSessionPtr session)
{
    auto request = InRoomPacketNewRequest(packet);

    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnNewRoomRequest] user {} sent a new room request "
                     "without being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    // if the user wants to create a new room, let it
    // this will remove the user from its current room
    // it should help mitigating the 'ghost room' issue,
    // where a room has users that aren't in it on the client's side
    if (curRoom != nullptr)
    {
        curRoom->RemoveSlotById(session->GetUser()->GetId());
        session->SetCurRoom(nullptr);
    }

    auto newRoom = std::make_shared<Room>(curChannel->GetNextRoomId(),
                                          request.Options, session);
    curChannel->AddRoom(newRoom);
    session->SetCurRoom(newRoom);

    Log::Debug("[OnNewRoomRequest] user {} created a new room with id {} and "
               "name {}\n",
               session->GetUser()->GetId(), newRoom->GetId(),
               newRoom->GetSettings().GetRoomName());
}

void OnJoinRoomRequest(PacketView& packet, ClientSessionPtr session)
{
    auto request = InRoomPacketJoinRequest(packet);

    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnJoinRoomRequest] user {} sent a new room request "
                     "without being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto desiredRoom = curChannel->FindRoomById(request.Id);

    if (desiredRoom == nullptr)
    {
        SendDialogBox(session, l10n::POPUP_ROOM_JOIN_FAILED_CLOSED);
        Log::Warning("[OnJoinRoomRequest] user {} tried to join a non existing "
                     "room with id {}\n",
                     session->GetUser()->GetId(), request.Id);
        return;
    }

    if (desiredRoom->GetFreeSlots() == 0)
    {
        SendDialogBox(session, l10n::POPUP_ROOM_JOIN_FAILED_CLOSED);
        return;
    }

    if (desiredRoom->IsPasswordProtected() == true &&
        desiredRoom->ComparePassword(request.Password) == false)
    {
        SendDialogBox(session, l10n::POPUP_ROOM_JOIN_FAILED_INVALID_PASSWD);
        return;
    }

    desiredRoom->Join(session);
    session->SetCurRoom(desiredRoom);

    Log::Debug("[OnJoinRoomRequest] user {} joined room with id {}\n",
               session->GetUser()->GetId(), desiredRoom->GetId());
}

void OnGameStartRequest(ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnGameStartRequest] user {} sent a game start request "
                     "without being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnGameStartRequest] user {} sent a game start request "
                     "without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    if (curRoom->GetHostUserId() == session->GetUser()->GetId())
    {
        curRoom->HostGameStart();
        Log::Debug("[OnGameStartRequest] host user {} launched a match in room "
                   "id {}\n",
                   session->GetUser()->GetId(), curRoom->GetId());
    }
    else if (curRoom->GetStatus() == RoomStatus::Ingame)
    {
        curRoom->GuestJoinGame(session->GetUser()->GetId());
        Log::Debug("[OnGameStartRequest] user {} joined an ingame match in "
                   "room id {}\n",
                   session->GetUser()->GetId(), curRoom->GetId());
    }
    else
    {
        Log::Warning(
            "[OnGameStartRequest] user {} tried to launch a match in room "
            "id {} without being a host\n",
            session->GetUser()->GetId(), curRoom->GetId());
    }
}

void OnLeaveRoomRequest(ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnLeaveRoomRequest] user {} sent a leave room request "
                     "without being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnLeaveRoomRequest] user {} sent a leave room request "
                     "without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto userId = session->GetUser()->GetId();

    if (curRoom->IsUserReady(userId) == true &&
        curRoom->IsCountingDown() == true)
    {
        // if the user is ready and a countdown is in progress, don't let the
        // user leave the room
        return;
    }

    curRoom->RemoveSlotById(userId);
    session->SetCurRoom(nullptr);

    Log::Debug("[OnLeaveRoomRequest] user {} left room id {}\n",
               session->GetUser()->GetId(), curRoom->GetId());

    SendChannelRoomList(session, curChannel);
}

void OnToggleReadyRequest(ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnToggleReadyRequest] user {} sent a toggle ready "
                     "request without being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnToggleReadyRequest] user {} sent a toggle ready "
                     "request without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto userId = session->GetUser()->GetId();

    auto [updated, newStatus] = curRoom->ToggleUserReadyStatus(userId);

    if (updated == false)
    {
        Log::Warning("[OnToggleReadyRequest] failed to set user's {} ready "
                     "status current room id is {}\n",
                     session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    Log::Debug("[OnToggleReadyRequest] user {} in room {} updated ready status "
               "to {}\n",
               session->GetUser()->GetId(), curRoom->GetId(), newStatus);
}

void OnUpdateSettingsRequest(PacketView& packet, ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning(
            "[OnUpdateSettingsRequest] user {} sent updated room settings "
            "request without being in a channel\n",
            session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning(
            "[OnUpdateSettingsRequest] user {} sent updated room settings "
            "request without being in a room\n",
            session->GetUser()->GetId());
        return;
    }

    auto userId = session->GetUser()->GetId();

    if (userId != curRoom->GetHostUserId())
    {
        Log::Warning(
            "[OnUpdateSettingsRequest] user {} sent updated room settings "
            "request without being the host of room {}\n",
            session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    if (curRoom->IsCountingDown() == true)
    {
        Log::Warning("[OnUpdateSettingsRequest] host user {} of room {} sent "
                     "updated room settings request during a countdown\n",
                     session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    auto reqPkt = InRoomPacketUpdateSettings(packet);

    curRoom->UpdateSettings(reqPkt.Flags, std::move(reqPkt.Settings));

    Log::Debug(
        "[OnUpdateSettingsRequest] host user {} of room {} updated settings\n",
        session->GetUser()->GetId(), curRoom->GetId());
}

void OnLeaveResultWindowRequest(ClientSessionPtr session)
{
    auto pkt = OutHostPacket::LeaveResultWindow();
    session->Send(std::move(pkt));
}

void OnSetTeamRequest(PacketView& packet, ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnSetTeamRequest] user {} sent new team request without "
                     "being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnSetTeamRequest] user {} sent new team request without "
                     "being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto userId = session->GetUser()->GetId();

    if (curRoom->IsUserReady(userId) == true)
    {
        SendSystemMessage(session, l10n::POPUP_ROOM_CHANGETEAM_FAILED);

        Log::Warning("[OnSetTeamRequest] user {} in room {} sent new team "
                     "request while ready\n",
                     session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    if (curRoom->GetSettings().AreBotsEnabled() == true &&
        userId != curRoom->GetHostUserId())
    {
        Log::Warning(
            "[OnSetTeamRequest] user {} in room {} sent new team request when "
            "bots are enabled and the user is not a host\n",
            session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    auto teamPkt = InRoomPacketSetTeamRequest(packet);

    curRoom->UpdateUserTeam(userId, teamPkt.Team);

    Log::Debug("[OnSetTeamRequest] user {} of room {} changed teams\n",
               session->GetUser()->GetId(), curRoom->GetId());
}

void OnCountdownRequest(PacketView& packet, ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnCountdownRequest] user {} sent a game start toggle "
                     "without being in a channel\n",
                     session->GetUser()->GetId());
        return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnCountdownRequest] user {} sent a game start toggle "
                     "without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto userId = session->GetUser()->GetId();

    if (userId != curRoom->GetHostUserId())
    {
        Log::Warning("[OnCountdownRequest] user {} in room {} sent a game "
                     "start toggle without being the host\n",
                     session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    if (curRoom->CanStartGame() == false)
    {
        SendSystemMessage(session, l10n::UI_ROOM_COUNTDOWN_FAILED_NOENEMY);

        Log::Warning("[OnCountdownRequest] user {} in room {} sent a game "
                     "start toggle, although it cannot start\n",
                     session->GetUser()->GetId(), curRoom->GetId());
        return;
    }

    auto countPkt = InRoomPacketCountdownRequest(packet);

    if (countPkt.CountingDown == true)
    {
        curRoom->ProgressCountdown(countPkt.CountNum);
        Log::Debug("[OnCountdownRequest] host user {} of room {} is counting "
                   "down at {} (host is at {})\n",
                   session->GetUser()->GetId(), curRoom->GetId(),
                   curRoom->GetCountdown(), countPkt.CountNum);
    }
    else
    {
        curRoom->StopCountdown();
        Log::Debug(
            "[OnCountdownRequest] host user {} of room {} stopped countdown\n",
            session->GetUser()->GetId(), curRoom->GetId());
    }
}

void OnRoomPacketRequest(PacketView& packet, ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[RoomHandler::OnRoomPacket] conn '{}' sent "
                     "room packet without session\n",
                     session->GetUniqueId());
        return;
    }

    auto roomPkt = InRoomPacket(packet);

    switch (roomPkt.Type)
    {
        case InRoomPacketType::NewRoomRequest:
            return OnNewRoomRequest(packet, session);
        case InRoomPacketType::JoinRoomRequest:
            return OnJoinRoomRequest(packet, session);
        case InRoomPacketType::GameStartRequest:
            return OnGameStartRequest(session);
        case InRoomPacketType::LeaveRoomRequest:
            return OnLeaveRoomRequest(session);
        case InRoomPacketType::ToggleReadyRequest:
            return OnToggleReadyRequest(session);
        case InRoomPacketType::UpdateSettings:
            return OnUpdateSettingsRequest(packet, session);
        case InRoomPacketType::OnCloseResultWindow:
            return OnLeaveResultWindowRequest(session);
        case InRoomPacketType::SetUserTeamRequest:
            return OnSetTeamRequest(packet, session);
        case InRoomPacketType::CountdownRequest:
            return OnCountdownRequest(packet, session);
    }

    Log::Warning(
        "[RoomHandler::OnRoomPacket] user {} sent unknown room packet\n",
        session->GetUser()->GetId());
}
