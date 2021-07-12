#ifndef __PACKET_VIEW_H_
#define __PACKET_VIEW_H_

#include "buffer/view.hpp"

#include "id.hpp"

/*
 * The game's base packet
 * Structure:
 * [signature - 1 byte]
 * [sequence - 1 byte]
 * [length - 2 bytes]
 * [packetId - 1 byte] -- NOTE: this is technically not part of the base packet
 */
class PacketView : public BufferView
{
public:
    PacketView(const std::span<const std::uint8_t> data);

    [[nodiscard]] inline std::uint32_t GetSequence() const noexcept
    {
        return this->m_Sequence;
    }

    [[nodiscard]] inline std::uint32_t GetLength() const noexcept
    {
        return this->m_Length;
    }

    [[nodiscard]] inline PacketId GetId() const noexcept { return this->m_Id; }

private:
    std::uint32_t m_Sequence;
    std::uint32_t m_Length;
    PacketId m_Id;
};

#endif  // __PACKET_VIEW_H_
