#include "packets/view.hpp"

PacketView::PacketView(const std::span<const std::uint8_t> data)
    : BufferView(data), m_Sequence(0), m_Length(0), m_Id(PacketId::Invalid)
{
    const std::uint8_t signature = this->Read<std::uint8_t>();

    if (signature != GAME_PACKET_SIGNATURE)
    {
        throw std::runtime_error("The packet's signature is invalid!");
    }

    this->m_Sequence = this->Read<std::uint8_t>();
    this->m_Length = this->Read<std::uint16_t>();
    this->m_Id = PacketId(this->Read<std::uint8_t>());
}
