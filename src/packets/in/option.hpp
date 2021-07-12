#ifndef __IN_PACKET_OPTION_H_
#define __IN_PACKET_OPTION_H_

#include "packets/optionshared.hpp"

class PacketView;

class InOptionPacket
{
public:
    InOptionPacket(PacketView& packet);

    OptionPacketType Type;
};

#endif  // __IN_PACKET_OPTION_H_
