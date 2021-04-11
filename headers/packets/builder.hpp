#ifndef __PACKET_BUILDER_H_
#define __PACKET_BUILDER_H_

#include "buffer/dynamicbuffer.hpp"
#include "packets/id.hpp"

class PacketBuilder
{
public:
    PacketBuilder(PacketId id, std::size_t initialSize = 0);

    [[nodiscard]] inline DynamicBuffer& GetBuffer() { return this->m_Buffer; }
    [[nodiscard]] std::vector<std::uint8_t>&& GetBufferOwnership();

    void SetSequence(std::uint8_t seq);
    void SetLength(std::uint16_t len);

    void Finish();

private:
    DynamicBuffer m_Buffer;
};

#endif  // __PACKET_BUILDER_H_
