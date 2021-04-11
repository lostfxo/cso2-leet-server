#ifndef __OUT_PKT_USERSTART_
#define __OUT_PKT_USERSTART_

#include "packets/builder.hpp"

class OutUserStartPacket
{
public:
    static PacketBuilder UserStart(std::uint32_t userId, std::string_view loginName,
                                   std::string_view userName,
                                   std::uint16_t holepunchPort);
};

#endif  // __OUT_PKT_USERSTART_
