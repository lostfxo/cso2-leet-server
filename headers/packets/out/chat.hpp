#ifndef __OUT_PKT_CHAT_
#define __OUT_PKT_CHAT_

#include "packets/builder.hpp"

class OutChatPacket
{
public:
    static PacketBuilder ChannelMessage(std::string_view msg,
                                        std::string_view sender, bool gm);
    static PacketBuilder DirectMessage(std::string_view msg,
                                       std::string_view sender,
                                       std::string_view receiver,
                                       std::uint8_t vipLevel, bool isReceiver,
                                       bool isGm);
    static PacketBuilder RoomMessage(std::string_view msg,
                                     std::string_view sender, std::uint8_t vipLevel,
                                     bool isGm);
    static PacketBuilder IngameGlobalMessage(std::string_view msg,
                                             std::string_view sender,
                                             std::uint8_t vipLevel, bool isGm);
    static PacketBuilder IngameTeamMessage(std::string_view msg,
                                           std::string_view sender,
                                           std::uint8_t vipLevel, bool isGm);

    static PacketBuilder DialogBox(std::string_view msg);
    static PacketBuilder SystemMessage(std::string_view msg);
};

#endif  // __OUT_PKT_CHAT_
