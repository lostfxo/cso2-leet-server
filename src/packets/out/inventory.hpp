#ifndef __OUT_PKT_INVENTORY_
#define __OUT_PKT_INVENTORY_

#include "cso2/shared.hpp"
#include "packets/builder.hpp"

class OutInventoryPacket
{
public:
    static PacketBuilder Create(cso2::InventoryPtr inv);
    static PacketBuilder AddItems(const std::span<cso2::InventoryItem> items);
};

#endif  // __OUT_PKT_INVENTORY_
