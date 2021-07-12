#include "packets/in/host/onplayerbuyitem.hpp"

#include "packets/view.hpp"

InHostPacketOnPlayerBuyItem::InHostPacketOnPlayerBuyItem(PacketView& packet)
{
    this->BuyerUserId = packet.Read<cso2::UserId_t>();
    std::uint8_t numItemsBought = packet.Read<std::uint8_t>();
    this->ItemsBought.reserve(numItemsBought);

    for (std::uint8_t i = 0; i < numItemsBought; i++)
    {
        this->ItemsBought.push_back(packet.Read<std::uint32_t>());
    }
}
