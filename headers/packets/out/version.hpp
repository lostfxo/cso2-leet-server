#ifndef __OUT_PKT_VERSION_
#define __OUT_PKT_VERSION_

#include "packets/builder.hpp"

class OutVersionPacket
{
public:
    static PacketBuilder Version(std::string_view hash, bool isBadHash);
};

#endif  // __OUT_PKT_VERSION_
