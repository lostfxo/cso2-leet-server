#include "packets/out/custom.hpp"

#include "cso2/definitions.hpp"
#include "packets/customshared.hpp"

PacketBuilder OutCustomPacket::SetUseRelay(bool enabled)
{
    PacketBuilder res(PacketId::Custom, 16);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(CustomPacketType::SetUdpRelay));
    bufRef.Write<std::uint8_t>(enabled ? 1 : 0);
    res.Finish();

    return res;
}
