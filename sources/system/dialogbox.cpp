#include "system/dialogbox.hpp"

#include "clientsession.hpp"
#include "packets/out/chat.hpp"

void SendDialogBox(ClientSessionPtr session, std::string_view msg)
{
    auto chatPkt = OutChatPacket::DialogBox(msg);
    session->Send(std::move(chatPkt));
}

void SendSystemMessage(ClientSessionPtr session, std::string_view msg)
{
    auto chatPkt = OutChatPacket::SystemMessage(msg);
    session->Send(std::move(chatPkt));
}
