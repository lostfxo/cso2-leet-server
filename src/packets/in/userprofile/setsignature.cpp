#include "packets/in/userprofile/setsignature.hpp"

#include "packets/view.hpp"

InUserProfilePacketSetSignature::InUserProfilePacketSetSignature(
    PacketView& packet)
{
    this->Signature = packet.ReadString();
}
