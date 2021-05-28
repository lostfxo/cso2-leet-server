#include "holepunch/inpacket/forward.hpp"

#include "buffer/view.hpp"

HolepunchInFwdPacket::HolepunchInFwdPacket(BufferView& data)
{
    this->m_TargetPortId = HolepunchPortId(data.Read<std::uint8_t>());
    this->m_TargetUserId = data.Read<std::uint32_t>();
    this->m_PacketDataView = data.ReadView(data.GetRemainingBytes());
}
