#include "handlers.hpp"

#include "channel/channel.hpp"
#include "channel/channelstorage.hpp"
#include "channel/shared.hpp"
#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "util/log.hpp"

#include "packets/in/requestroomlist.hpp"
#include "packets/out/lobby.hpp"
#include "packets/out/serverlist.hpp"

void OnChannelListPacketRequest(PacketView& /*packet*/,
                                ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnChannelListPacketRequest] conn '{}' tried to get "
                     "channel list without session\n",
                     session->GetUniqueId());
        return;
    }

    auto channelsPkt = OutServerListPacket::ServerList(g_Channels.GetGroups());
    session->Send(std::move(channelsPkt));

    Log::Debug("[OnChannelListPacketRequest] sent channel list to user '{}'\n",
               session->GetUser()->GetId());
}

void OnRoomListPacketRequest(PacketView& packet, ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnRoomListPacketRequest] conn '{}' tried "
                     "to get channel list without session\n",
                     session->GetUniqueId());
        return;
    }

    auto listPkt = InRequestRoomListPacket(packet);

    auto chanGroup = g_Channels.GetGroupByIndex(listPkt.ChannelGroupIndex);

    if (chanGroup == nullptr)
    {
        Log::Warning("[OnRoomListPacketRequest] user {} requested "
                     "non existing channel group with index {}\n",
                     session->GetUser()->GetId(), listPkt.ChannelGroupIndex);
        return;
    }

    auto channel = chanGroup->GetChannelById(listPkt.ChannelIndex);

    if (channel == nullptr)
    {
        Log::Warning("[OnRoomListPacketRequest] user {} requested "
                     "non existing channel with index {} from group {}\n",
                     session->GetUser()->GetId(), listPkt.ChannelIndex,
                     listPkt.ChannelGroupIndex);
        return;
    }

    if (session->GetCurChannel() == channel)
    {
        channel->RemoveSessionFromChannel(session);
    }

    channel->AddSessionToChannel(session);

    SendChannelRoomList(session, channel);
}
