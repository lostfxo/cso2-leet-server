#ifndef _HOLEPUNCH_PACKET_FORWARD_
#define _HOLEPUNCH_PACKET_FORWARD_

#include <span>

#include "holepunch/shared.hpp"

class BufferView;

class HolepunchInFwdPacket
{
public:
    HolepunchInFwdPacket(BufferView& data);

    // these members are memory aligned
    std::span<const uint8_t> m_PacketDataView;
    std::uint32_t m_TargetUserId;
    HolepunchPortId m_TargetPortId;
};

#endif  // _HOLEPUNCH_PACKET_FORWARD_
