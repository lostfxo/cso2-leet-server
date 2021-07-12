#ifndef __HOLEPUNCH_INPACKET_H_
#define __HOLEPUNCH_INPACKET_H_

#include "buffer/view.hpp"
#include "holepunch/shared.hpp"

class HolepunchInPacket : public BufferView
{
public:
    HolepunchInPacket(const std::span<const std::uint8_t> data);

    [[nodiscard]] inline bool IsHeartbeat() const noexcept
    {
        return this->m_Type > HolepunchPacketType::Punch &&
               this->m_Type <= HolepunchPacketType::HeartbeatForSourceTV;
    }

    // these members are memory aligned
    std::uint32_t m_UserId;
    std::uint8_t m_Signature;
    HolepunchPacketType m_Type;
};

#endif  // __HOLEPUNCH_INPACKET_H_
