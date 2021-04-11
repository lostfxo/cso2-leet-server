#ifndef __IN_PACKET_CHAT_H_
#define __IN_PACKET_CHAT_H_

#include <string>

#include "packets/chatshared.hpp"

class PacketView;

class InChatPacket
{
public:
    InChatPacket(PacketView& packet);

    ChatPacketType Type;
    std::string Destination;
    std::string Message;
};

#endif  // __IN_PACKET_CHAT_H_
