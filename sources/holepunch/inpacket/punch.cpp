#include "holepunch/inpacket/punch.hpp"

#include "buffer/view.hpp"

HolepunchInPunchPacket::HolepunchInPunchPacket(BufferView& data)
{
    this->m_PortId = HolepunchPortId(data.Read<std::uint8_t>());
    this->m_IpAddress = ~data.Read<std::uint32_t>(true);
    this->m_PortNum = data.Read<std::uint16_t>();
}
