#ifndef _HOLEPUNCH_PACKET_PUNCH_
#define _HOLEPUNCH_PACKET_PUNCH_

#include "holepunch/shared.hpp"

class BufferView;

class HolepunchInPunchPacket
{
public:
    HolepunchInPunchPacket(BufferView& data);

    // these members are memory aligned
    std::uint32_t m_IpAddress;
    std::uint16_t m_PortNum;
    HolepunchPortId m_PortId;
};

#endif  // _HOLEPUNCH_PACKET_PUNCH_
