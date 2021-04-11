#include "handlers.hpp"

#include "activesessions.hpp"
#include "channel/channel.hpp"
#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "packets/chatshared.hpp"
#include "packets/view.hpp"
#include "room/room.hpp"
#include "util/log.hpp"

#include "packets/in/chat.hpp"
#include "packets/out/chat.hpp"

void OnChatChannelMsgRequest(const InChatPacket& chatPkt,
                             ClientSessionPtr session)
{
    auto curChannel = session->GetCurChannel();

    if (curChannel == nullptr)
    {
        Log::Warning("[OnChatChannelMsgRequest] user {} sent a channel message "
                     "without being in one\n",
                     session->GetUser()->GetId());
        return;
    }

    auto sessionUser = session->GetUser();

    auto outPkt = OutChatPacket::ChannelMessage(chatPkt.Message,
                                                sessionUser->GetUserName(),
                                                sessionUser->IsGameMaster());

    for (const auto& session : curChannel->GetSessions())
    {
        session->Send(std::move(outPkt));
    }
}

void OnChatDirectMsgRequest(const InChatPacket& chatPkt,
                            ClientSessionPtr session)
{
    if (chatPkt.Destination.empty() == true)
    {
        Log::Warning("[OnChatDirectMsgRequest] user {} tried to send a direct "
                     "message without destination\n",
                     session->GetUser()->GetId());
        return;
    }

    auto sessionUser = session->GetUser();

    if (chatPkt.Destination == sessionUser->GetUserName())
    {
        Log::Warning("[OnChatDirectMsgRequest] user {} tried to send a direct "
                     "message to itself\n",
                     session->GetUser()->GetId());
        return;
    }

    auto destSession = g_Sessions.FindSessionByUserName(chatPkt.Destination);

    if (destSession == nullptr)
    {
        Log::Warning("[OnChatDirectMsgRequest] user {} tried to send a message "
                     "to non existing destination\n",
                     session->GetUser()->GetId());
        Log::Debug("[OnChatDirectMsgRequest] the destination's name is {}\n",
                   chatPkt.Destination);
        return;
    }

    auto destUser = destSession->GetUser();

    auto pktForSrc = OutChatPacket::DirectMessage(
        chatPkt.Message, sessionUser->GetUserName(), destUser->GetUserName(),
        sessionUser->GetVipLevel(), false, sessionUser->IsGameMaster());
    session->Send(std::move(pktForSrc));

    auto pktForDest = OutChatPacket::DirectMessage(
        chatPkt.Message, sessionUser->GetUserName(), destUser->GetUserName(),
        sessionUser->GetVipLevel(), true, sessionUser->IsGameMaster());
    destSession->Send(std::move(pktForDest));
}

void OnChatRoomMsgRequest(const InChatPacket& chatPkt, ClientSessionPtr session)
{
    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnChatRoomMsgRequest] user {} tried to send a room "
                     "message without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto sessionUser = session->GetUser();

    if (curRoom->IsUserIngame(sessionUser->GetId()) == true)
    {
        Log::Warning("[OnChatRoomMsgRequest] user {} tried to send a room "
                     "message while ingame\n",
                     session->GetUser()->GetId());
        return;
    }

    auto msgPkt = OutChatPacket::RoomMessage(
        chatPkt.Message, sessionUser->GetUserName(), sessionUser->GetVipLevel(),
        sessionUser->IsGameMaster());
    session->Send(std::move(msgPkt));
}

void OnChatIngameGlobalMsgRequest(const InChatPacket& chatPkt,
                                  ClientSessionPtr session)
{
    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnChatIngameGlobalMsgRequest] user {} tried to send a "
                     "room message without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto sessionUser = session->GetUser();

    if (curRoom->IsUserIngame(sessionUser->GetId()) == false)
    {
        Log::Warning("[OnChatIngameGlobalMsgRequest] user {} tried to send a "
                     "room message without being ingame\n",
                     session->GetUser()->GetId());
        return;
    }

    auto msgPkt = OutChatPacket::IngameGlobalMessage(
        chatPkt.Message, sessionUser->GetUserName(), sessionUser->GetVipLevel(),
        sessionUser->IsGameMaster());

    for (const auto& slot : curRoom->GetSlots())
    {
        slot.GetSession()->Send(std::move(msgPkt));
    }
}

void OnChatIngameTeamMsgRequest(const InChatPacket& chatPkt,
                                ClientSessionPtr session)
{
    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnChatIngameTeamMsgRequest] user {} tried to send a "
                     "room message without being in a room\n",
                     session->GetUser()->GetId());
        return;
    }

    auto sessionUser = session->GetUser();

    if (curRoom->IsUserIngame(sessionUser->GetId()) == false)
    {
        Log::Warning("[OnChatIngameTeamMsgRequest] user {} tried to send a "
                     "room message without being ingame\n",
                     session->GetUser()->GetId());
        return;
    }

    auto sessionSlot = curRoom->FindSlotById(sessionUser->GetId());
    Assert(sessionSlot != nullptr);

    auto msgPkt = OutChatPacket::IngameGlobalMessage(
        chatPkt.Message, sessionUser->GetUserName(), sessionUser->GetVipLevel(),
        sessionUser->IsGameMaster());

    for (const auto& slot : curRoom->GetSlots())
    {
        if (slot.GetTeam() == sessionSlot->GetTeam())
        {
            slot.GetSession()->Send(std::move(msgPkt));
        }
    }
}

void OnChatPacketRequest(PacketView& packet, ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnChatPacketRequest] conn '{}' sent favorite "
                     "packet without session\n",
                     session->GetUniqueId());
        return;
    }

    auto chatPkt = InChatPacket(packet);

    if (chatPkt.Message.empty() || chatPkt.Message[0] == '\0')
    {
        Log::Warning("[OnChatPacketRequest] user {} sent an empty message\n",
                     session->GetUser()->GetId());
        return;
    }

    switch (chatPkt.Type)
    {
        case ChatPacketType::Channel:
            OnChatChannelMsgRequest(packet, session);
            break;
        case ChatPacketType::DirectMessage:
            OnChatDirectMsgRequest(packet, session);
            break;
        case ChatPacketType::Room:
            OnChatRoomMsgRequest(packet, session);
            break;
        case ChatPacketType::IngameGlobal:
            OnChatIngameGlobalMsgRequest(packet, session);
            break;
        case ChatPacketType::IngameTeam:
            OnChatIngameTeamMsgRequest(packet, session);
            break;
        default:
            Log::Warning("[OnChatPacketRequest] user {} sent unknown chat "
                         "packet sub type {}\n",
                         session->GetUser()->GetId(), chatPkt.Type);
    }
}
