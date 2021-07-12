#include "packets/in/userprofile/setavatar.hpp"

#include "packets/view.hpp"

InUserProfilePacketSetAvatar::InUserProfilePacketSetAvatar(PacketView& packet)
{
    this->AvatarId = packet.Read<std::uint16_t>();
}
