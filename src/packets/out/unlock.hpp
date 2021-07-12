#ifndef __OUT_PKT_UNLOCK_
#define __OUT_PKT_UNLOCK_

#include "cso2/definitions.hpp"
#include "packets/builder.hpp"

enum class UnlockPacketType
{
    SetUnlocked = 1
};

class OutUnlockPacket
{
public:
    static PacketBuilder SetUnlocks(
        const std::span<cso2::UnlockItem> items,
        const std::span<cso2::UnlockProgress> progresses);
};

#endif  // __OUT_PKT_UNLOCK_
