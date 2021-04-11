#ifndef __IN_PACKET_HOST_ONPLAYERBUYITEM_H_
#define __IN_PACKET_HOST_ONPLAYERBUYITEM_H_

#include <vector>

#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"

class PacketView;

class InHostPacketOnPlayerBuyItem
{
public:
    InHostPacketOnPlayerBuyItem(PacketView& packet);

    cso2::UserId_t BuyerUserId;
    std::vector<std::uint32_t> ItemsBought;
};

#endif  // __IN_PACKET_HOST_ONPLAYERBUYITEM_H_
