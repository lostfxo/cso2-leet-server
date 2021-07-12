#ifndef __HOLEPUNCH_OUTPACKET_H_
#define __HOLEPUNCH_OUTPACKET_H_

#include "buffer/staticbuffer.hpp"
#include "holepunch/shared.hpp"

class HolepunchOutPacket : public StaticBuffer<4>
{
public:
    HolepunchOutPacket(HolepunchPortId updatedPortId);
};

#endif  // __HOLEPUNCH_OUTPACKET_H_
