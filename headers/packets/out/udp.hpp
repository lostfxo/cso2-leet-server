#ifndef __OUT_PKT_UDP_
#define __OUT_PKT_UDP_

#include "packets/builder.hpp"

class OutUdpPacket
{
public:
    static PacketBuilder Udp(std::uint32_t userId, std::uint32_t ipAddress,
                             std::uint16_t portNum, bool host);
};

#endif  // __OUT_PKT_UDP_
