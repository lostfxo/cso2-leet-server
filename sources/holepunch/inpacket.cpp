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

    if (this->IsHeartbeat() == false)
    {
        this->m_UserId = this->Read<std::uint32_t>();
        this->m_PortId = this->Read<std::uint16_t>();
        this->m_IpAddress = ~this->Read<std::uint32_t>(true);
        this->m_PortNum = this->Read<std::uint16_t>();
    }
    else
    {
        // this is a heartbeat packet
        // TODO: reverse this?
    }
}
