#ifndef __OUT_PKT_OPTION_
#define __OUT_PKT_OPTION_

#include "cso2/shared.hpp"
#include "packets/builder.hpp"

class OutOptionPacket
{
public:
    static PacketBuilder SetBuyMenu(const cso2::BuyMenuPtr buyMenu);
};

#endif  // __OUT_PKT_OPTION_
