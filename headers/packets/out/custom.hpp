#ifndef __OUT_PKT_CUSTOM_
#define __OUT_PKT_CUSTOM_

#include "packets/builder.hpp"

class OutCustomPacket
{
public:
    static PacketBuilder SetUseRelay(bool enabled);
};

#endif  // __OUT_PKT_CUSTOM_
