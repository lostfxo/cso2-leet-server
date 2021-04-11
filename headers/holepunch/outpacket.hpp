#ifndef __HOLEPUNCH_OUTPACKET_H_
#define __HOLEPUNCH_OUTPACKET_H_

#include "buffer/staticbuffer.hpp"

class HolepunchOutPacket : public StaticBuffer<4>
{
public:
    HolepunchOutPacket(std::uint16_t updatedPortId);
};

#endif  // __HOLEPUNCH_OUTPACKET_H_
