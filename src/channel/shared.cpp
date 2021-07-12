#include "channel/shared.hpp"

#include "channel/channel.hpp"
#include "clientsession.hpp"

#include "packets/out/lobby.hpp"
#include "packets/out/roomlist.hpp"

void SendChannelRoomList(ClientSessionPtr session, ChannelPtr channel)
{
    auto joinPkt = OutLobbyPacket::JoinRoomList();
    auto outListPkt = OutRoomListPacket::SendRoomList(channel->GetRooms());

    session->Send(std::move(joinPkt));
    session->Send(std::move(outListPkt));
}
