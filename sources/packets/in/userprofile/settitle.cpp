#include "packets/in/userprofile/settitle.hpp"

#include "packets/view.hpp"

InUserProfilePacketSetTitle::InUserProfilePacketSetTitle(PacketView& packet)
{
    this->TitleId = packet.Read<std::uint16_t>();
}
