#ifndef __HOLEPUNCH_INPACKET_H_
#define __HOLEPUNCH_INPACKET_H_

#include "buffer/view.hpp"

class HolepunchInPacket : public BufferView
{
public:
    HolepunchInPacket(const std::span<const std::uint8_t> data);

    [[nodiscard]] inline bool IsHeartbeat() const noexcept
    {
        return this->GetDataViewSize() == 6;
    }

    // these members are memory aligned
    std::uint32_t m_UserId;
    std::uint32_t m_IpAddress;
    std::uint16_t m_PortId;
    std::uint16_t m_PortNum;

    std::uint8_t m_Signature;
};

#endif  // __HOLEPUNCH_INPACKET_H_
