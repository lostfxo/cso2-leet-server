#ifndef __OUT_PKT_HOST_H_
#define __OUT_PKT_HOST_H_

#include "cso2/shared.hpp"
#include "packets/builder.hpp"

class OutHostPacket
{
public:
    static PacketBuilder GameStart(std::uint32_t hostUserId);
    static PacketBuilder HostJoin(std::uint32_t hostUserId);
    static PacketBuilder HostStop();
    static PacketBuilder LeaveResultWindow();
    static PacketBuilder ItemUsed(std::uint32_t userId, std::uint32_t itemId,
                                  std::uint8_t remainingUses);
    static PacketBuilder SetInventory(std::uint32_t userId,
                                      const cso2::InventoryItemsArray& items);
    static PacketBuilder SetLoadout(std::uint32_t userId,
                                    const cso2::CosmeticsPtr cosmetics,
                                    const cso2::LoadoutsArray& loadouts);
    static PacketBuilder SetBuyMenu(std::uint32_t userId,
                                    const cso2::BuyMenu& buyMenu);
};

#endif  // __OUT_PKT_HOST_H_
