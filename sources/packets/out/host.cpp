#include "packets/out/host.hpp"

#include <gsl/gsl>

#include "cso2/buymenu.hpp"
#include "cso2/cosmetics.hpp"
#include "cso2/definitions.hpp"
#include "cso2/inventory.hpp"
#include "cso2/loadout.hpp"

enum class OutHostPacketType : std::uint8_t
{
    GameStart = 0,  // when a host starts a new game
    HostJoin = 1,   // when someone joins some host's game
    HostStop = 3,
    LeaveResultWindow = 4,

    SetInventory =
        101,  // there are 2 or 3 other host packet types that send this
    ItemUsed = 105,
    SetLoadout = 107,
    SetBuyMenu = 111
};

inline void WriteBuySubMenu(DynamicBuffer& buf, const cso2::BuyCategory& items)
{
    std::uint8_t curItem = 0;

    buf.Write(gsl::narrow<std::uint8_t>(items.size()));

    for (auto item : items)
    {
        buf.Write<std::uint8_t>(curItem++);
        buf.Write<std::uint32_t>(item);
    }
}

inline void WriteLoadoutItem(DynamicBuffer& buf, std::uint32_t itemNum,
                             std::uint8_t curItem)
{
    buf.Write<std::uint8_t>(curItem);
    buf.Write<std::uint32_t>(itemNum);
}

PacketBuilder OutHostPacket::GameStart(std::uint32_t hostUserId)
{
    PacketBuilder res(PacketId::Host, 24);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::GameStart));
    buf.Write<std::uint32_t>(hostUserId);

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::HostJoin(std::uint32_t hostUserId)
{
    PacketBuilder res(PacketId::Host, 24);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::HostJoin));
    buf.Write<std::uint32_t>(hostUserId);
    buf.Write<std::uint64_t>(0);  // unk

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::HostStop()
{
    PacketBuilder res(PacketId::Host, 24);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::HostStop));

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::LeaveResultWindow()
{
    PacketBuilder res(PacketId::Host, 24);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::LeaveResultWindow));

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::ItemUsed(std::uint32_t userId,
                                      std::uint32_t itemId,
                                      std::uint8_t remainingUses)
{
    PacketBuilder res(PacketId::Host, 24);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::ItemUsed));

    buf.Write<std::uint32_t>(userId);
    buf.Write<std::uint32_t>(itemId);
    buf.Write<std::uint8_t>(remainingUses);

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::SetInventory(
    std::uint32_t userId, const cso2::InventoryItemsArray& items)
{
    PacketBuilder res(PacketId::Host, 8196);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::SetInventory));

    buf.Write<std::uint32_t>(userId);
    // writes somewhere to CGameClient
    buf.Write<std::uint8_t>(0);  // unk00
    buf.Write(gsl::narrow<std::uint16_t>(items.size()));

    for (const auto& item : items)
    {
        buf.Write<std::uint32_t>(item.ItemId);
        buf.Write<std::uint16_t>(item.Ammount);
    }

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::SetLoadout(std::uint32_t userId,
                                        const cso2::CosmeticsPtr cosmetics,
                                        const cso2::LoadoutsArray& loadouts)
{
    PacketBuilder res(PacketId::Host, 512);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::SetLoadout));

    buf.Write<std::uint32_t>(userId);

    std::uint8_t curItem = 0;

    buf.Write<std::uint8_t>(8);  // num of cosmetics
    WriteLoadoutItem(buf, cosmetics->GetCtItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetTerItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetHeadItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetGloveItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetBackItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetStepsItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetCardItem(), curItem++);
    WriteLoadoutItem(buf, cosmetics->GetSprayItem(), curItem++);

    buf.Write<std::uint8_t>(3);  // num of loadouts

    for (const auto& loadout : loadouts)
    {
        curItem = 0;

        buf.Write<std::uint8_t>(16);  // num of loadout slots
        WriteLoadoutItem(buf, loadout->GetPrimaryWeapon(), curItem++);
        WriteLoadoutItem(buf, loadout->GetSecondaryWeapon(), curItem++);
        WriteLoadoutItem(buf, loadout->GetMelee(), curItem++);
        WriteLoadoutItem(buf, loadout->GetHeGrenade(), curItem++);
        WriteLoadoutItem(buf, loadout->GetSmoke(), curItem++);
        WriteLoadoutItem(buf, loadout->GetFlash(), curItem++);

        // FIXME: do we need this?
        while (curItem < 16)
        {
            WriteLoadoutItem(buf, 0, curItem++);
        }
    }

    buf.Write<std::uint8_t>(0);  // unk00

    res.Finish();

    return res;
}

PacketBuilder OutHostPacket::SetBuyMenu(std::uint32_t userId,
                                        const cso2::BuyMenu& buyMenu)
{
    PacketBuilder res(PacketId::Host, 1024);
    auto& buf = res.GetBuffer();

    buf.Write(std::uint8_t(OutHostPacketType::SetBuyMenu));

    buf.Write<std::uint32_t>(userId);

    buf.Write<std::uint16_t>(369);  // buy menu's byte length
    buf.Write<std::uint8_t>(0);     // unk00

    WriteBuySubMenu(buf, buyMenu.GetPistols());
    WriteBuySubMenu(buf, buyMenu.GetShotguns());
    WriteBuySubMenu(buf, buyMenu.GetSmgs());
    WriteBuySubMenu(buf, buyMenu.GetRifles());
    WriteBuySubMenu(buf, buyMenu.GetSnipers());
    WriteBuySubMenu(buf, buyMenu.GetMachineGuns());
    WriteBuySubMenu(buf, buyMenu.GetMelees());
    WriteBuySubMenu(buf, buyMenu.GetEquipment());

    res.Finish();

    return res;
}
