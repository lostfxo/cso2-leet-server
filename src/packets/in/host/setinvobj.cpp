#include "packets/in/host/setinvobj.hpp"

#include "packets/view.hpp"

InHostPacketSetInvObject::InHostPacketSetInvObject(PacketView& packet)
{
    this->UserId = packet.Read<std::uint32_t>();
}
