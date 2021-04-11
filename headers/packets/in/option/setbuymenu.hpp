#ifndef __IN_PACKET_OPTION_SETBUYMENU_H_
#define __IN_PACKET_OPTION_SETBUYMENU_H_

#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"

class PacketView;

class InOptionPacketSetBuyMenu
{
public:
    InOptionPacketSetBuyMenu(PacketView& packet);

    cso2::BuyCategoriesGroup BuyMenu;

    std::uint16_t MenuByteLength;
    std::uint8_t unk00;
};

#endif  // __IN_PACKET_OPTION_SETBUYMENU_H_
