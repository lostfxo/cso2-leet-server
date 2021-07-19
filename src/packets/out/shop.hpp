#ifndef __OUT_PKT_SHOP_
#define __OUT_PKT_SHOP_

#include "cso2/definitions.hpp"
#include "packets/builder.hpp"

class OutShopPacket
{
public:
    static PacketBuilder Create(const std::vector<cso2::ShopItem>& items);
};

#endif  // __OUT_PKT_SHOP_
