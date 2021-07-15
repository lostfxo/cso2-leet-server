#include "packets/builder.hpp"

#include "util/number.hpp"
#include <stdexcept>

PacketBuilder::PacketBuilder(PacketId id, std::size_t initialSize /*= 0*/)
    : m_Buffer(initialSize)
{
    const std::array<std::uint8_t, GAME_PACKET_HEADER_LENGTH> emptyHeader = {
        GAME_PACKET_SIGNATURE, 0, 0, 0, std::uint8_t(id)
    };
    this->m_Buffer.WriteArray(emptyHeader);
}

std::vector<std::uint8_t>&& PacketBuilder::GetBufferOwnership()
{
    auto&& data = this->m_Buffer.GetDataOwnership();

    if (data.size() < GAME_PACKET_HEADER_LENGTH)
    {
        throw std::runtime_error(
            "Tried to get ownership of a too small buffer");
    }

    return std::move(data);
}

void PacketBuilder::SetSequence(std::uint8_t seq)
{
    this->m_Buffer.WriteToOffset(seq, 1);
}

void PacketBuilder::SetLength(std::uint16_t len)
{
    this->m_Buffer.WriteToOffset(len, 2);
}

void PacketBuilder::Finish()
{
    this->SetLength(util::FastNarrow<std::uint16_t>(this->m_Buffer.GetCurOffset() -
                                               GAME_PACKET_HEADER_LENGTH + 1));
}
