#include "packets/out/udp.hpp"

PacketBuilder OutUdpPacket::Udp(std::uint32_t userId, std::uint32_t ipAddress,
                                std::uint16_t portNum, bool host)

{
    PacketBuilder res(PacketId::Udp);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint8_t>(1);  // unk00
    buf.Write<std::uint8_t>(host);
    buf.Write<std::uint32_t>(userId);
    buf.Write<std::uint32_t>(ipAddress, true);
    buf.Write<std::uint16_t>(portNum);

    res.Finish();

    return res;
}
