#include "packets/in/userprofile.hpp"

#include "packets/view.hpp"

InUserProfilePacket::InUserProfilePacket(PacketView& packet)
{
    this->Type = UserProfilePacketType(packet.Read<std::uint8_t>());
}
