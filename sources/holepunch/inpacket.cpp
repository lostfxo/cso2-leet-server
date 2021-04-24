#include "holepunch/inpacket.hpp"

#include "holepunch/shared.hpp"

HolepunchInPacket::HolepunchInPacket(const std::span<const std::uint8_t> data)
    : BufferView(data)
{
    this->m_Signature = this->Read<std::uint8_t>();

    if (this->m_Signature != HOLEPUNCH_PACKET_SIGNATURE)
    {
        throw std::runtime_error("The holepunch packet signature is invalid");
    }

    this->m_UserId = this->Read<std::uint32_t>();
    this->m_Type = HolepunchPacketType(this->Read<std::uint8_t>());

    if (this->m_Type == HolepunchPacketType::Punch)
    {
        this->m_PortId = HolepunchPortId(this->Read<std::uint8_t>());
        this->m_IpAddress = ~this->Read<std::uint32_t>(true);
        this->m_PortNum = this->Read<std::uint16_t>();
    }
}
