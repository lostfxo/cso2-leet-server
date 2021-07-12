#include "packets/out/inventory.hpp"

#include <gsl/gsl>

#include "cso2/inventory.hpp"

inline void WriteInventoryItem(DynamicBuffer& buf, std::uint32_t id,
                               std::uint16_t ammount, std::uint16_t index)
{
    buf.Write<std::uint16_t>(index);
    buf.Write<std::uint8_t>(1);
    buf.Write<std::uint32_t>(id);
    buf.Write<std::uint16_t>(ammount);
    buf.Write<std::uint8_t>(1);  // item type
    buf.Write<std::uint8_t>(0);
    buf.Write<std::uint64_t>(0);
}

PacketBuilder OutInventoryPacket::Create(cso2::InventoryPtr inv)
{
    PacketBuilder res(PacketId::Inventory_Create, 16384);
    auto& bufRef = res.GetBuffer();

    const auto& items = inv->GetItems();

    bufRef.Write(gsl::narrow<std::uint16_t>(items.size()));

    std::uint16_t nextItemIndex = 0;

    for (const auto& item : items)
    {
        WriteInventoryItem(bufRef, item.ItemId, item.Ammount, nextItemIndex++);
    }

    res.Finish();

    return res;
}

PacketBuilder OutInventoryPacket::AddItems(
    const std::span<cso2::InventoryItem> items)
{
    PacketBuilder res(PacketId::Inventory_Create, 1024);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(gsl::narrow<std::uint16_t>(items.size()));

    std::uint16_t nextItemIndex = 0;

    for (const auto& item : items)
    {
        WriteInventoryItem(bufRef, item.ItemId, item.Ammount, nextItemIndex++);
    }

    res.Finish();

    return res;
}
