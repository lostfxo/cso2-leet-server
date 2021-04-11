#include "packets/in/option.hpp"

#include "packets/view.hpp"

InOptionPacket::InOptionPacket(PacketView& packet)
{
    this->Type = static_cast<OptionPacketType>(packet.Read<std::uint8_t>());
}
