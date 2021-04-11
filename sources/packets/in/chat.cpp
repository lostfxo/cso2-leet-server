#include "packets/in/chat.hpp"

#include "packets/view.hpp"

InChatPacket::InChatPacket(PacketView& packet)
{
    this->Type = ChatPacketType(packet.Read<std::uint8_t>());

    if (this->Type == ChatPacketType::DirectMessage)
    {
        this->Destination = packet.ReadString();
    }

    this->Message = packet.ReadLongString();
}
