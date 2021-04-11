#include "packets/out/unlock.hpp"

#include <gsl/gsl>

PacketBuilder OutUnlockPacket::SetUnlocks(
    const std::span<cso2::UnlockItem> items,
    const std::span<cso2::UnlockProgress> progresses)
{
    PacketBuilder res(PacketId::Unlock, 128);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(UnlockPacketType::SetUnlocked));

    // build unlock weapons info
    buf.Write(gsl::narrow<std::uint16_t>(items.size()));

    std::uint32_t nextItemIndex = 1;

    for (const auto& item : items)
    {
        buf.Write(item.ItemId);
        buf.Write(nextItemIndex++);
        buf.Write(std::uint8_t(item.Currency));
        buf.Write(item.Price);
    }

    // build killNum
    buf.Write(gsl::narrow<std::uint16_t>(progresses.size()));

    for (const auto& progress : progresses)
    {
        buf.Write(progress.ParentItemId);
        buf.Write(progress.TargetItemId);
        buf.Write(progress.Kills);
        buf.Write<std::uint32_t>(0);
        buf.Write<std::uint16_t>(0);
    }

    // build ID list
    buf.Write<std::uint16_t>(1);  // ID list array length,to be continued...
    buf.Write<std::uint32_t>(1);  // test item id

    res.Finish();

    return res;
}
