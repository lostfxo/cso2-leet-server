#include "packets/out/chat.hpp"

#include "cso2/definitions.hpp"
#include "packets/chatshared.hpp"

inline void WriteChatMsg(DynamicBuffer& pktBuf, std::string_view msg,
                         std::string_view sender, std::uint8_t vipLevel)
{
    pktBuf.WriteString(sender);
    pktBuf.Write<std::uint8_t>(vipLevel != 0 ? 1 : 0);
    pktBuf.Write<std::uint8_t>(vipLevel);
    pktBuf.WriteString(msg);
}

PacketBuilder OutChatPacket::ChannelMessage(std::string_view msg,
                                            std::string_view sender, bool gm)
{
    PacketBuilder res(PacketId::Chat, 512);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::Channel));
    bufRef.Write<std::uint8_t>(gm ? 1 : 0);
    WriteChatMsg(bufRef, msg, sender, 0);
    res.Finish();

    return res;
}

PacketBuilder OutChatPacket::DirectMessage(std::string_view msg,
                                           std::string_view sender,
                                           std::string_view receiver,
                                           std::uint8_t vipLevel, bool isReceiver,
                                           bool isGm)
{
    PacketBuilder res(PacketId::Chat, 512);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::DirectMessage));
    bufRef.Write<std::uint8_t>(isGm ? 1 : 0);
    bufRef.Write<std::uint8_t>(isReceiver ? 1 : 0);
    WriteChatMsg(bufRef, msg, isReceiver ? sender : receiver, vipLevel);
    res.Finish();

    return res;
}

PacketBuilder OutChatPacket::RoomMessage(std::string_view msg,
                                         std::string_view sender,
                                         std::uint8_t vipLevel, bool isGm)
{
    PacketBuilder res(PacketId::Chat, 512);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::Room));
    bufRef.Write<std::uint8_t>(isGm ? 1 : 0);
    WriteChatMsg(bufRef, msg, sender, vipLevel);
    res.Finish();

    return res;
}

PacketBuilder OutChatPacket::IngameGlobalMessage(std::string_view msg,
                                                 std::string_view sender,
                                                 std::uint8_t vipLevel, bool isGm)
{
    PacketBuilder res(PacketId::Chat, 512);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::IngameGlobal));
    bufRef.Write<std::uint8_t>(isGm ? 1 : 0);
    WriteChatMsg(bufRef, msg, sender, vipLevel);
    res.Finish();

    return res;
}

PacketBuilder OutChatPacket::IngameTeamMessage(std::string_view msg,
                                               std::string_view sender,
                                               std::uint8_t vipLevel, bool isGm)
{
    PacketBuilder res(PacketId::Chat, 512);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::IngameTeam));
    bufRef.Write<std::uint8_t>(isGm ? 1 : 0);
    WriteChatMsg(bufRef, msg, sender, vipLevel);
    res.Finish();

    return res;
}

PacketBuilder OutChatPacket::DialogBox(std::string_view msg)
{
    PacketBuilder res(PacketId::Chat, 256);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::DialogBox));
    bufRef.WriteLongString(msg);
    res.Finish();

    return res;
}

PacketBuilder OutChatPacket::SystemMessage(std::string_view msg)
{
    PacketBuilder res(PacketId::Chat, 256);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ChatPacketType::System));
    bufRef.WriteLongString(msg);
    res.Finish();

    return res;
}
