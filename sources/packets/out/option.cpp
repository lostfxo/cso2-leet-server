#include "packets/out/option.hpp"

#include <gsl/gsl>

#include "cso2/buymenu.hpp"
#include "packets/optionshared.hpp"

void WriteBuyCategory(DynamicBuffer& buf, const cso2::BuyCategory& category)
{
    buf.Write(gsl::narrow<std::uint8_t>(category.size()));

    std::uint8_t nextItemIndex = 0;

    for (auto entry : category)
    {
        buf.Write(nextItemIndex);
        buf.Write<std::uint32_t>(entry);
    }
}

PacketBuilder OutOptionPacket::SetBuyMenu(const cso2::BuyMenuPtr buyMenu)
{
    PacketBuilder res(PacketId::Option, 384);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(OptionPacketType::SetBuyMenu));

    bufRef.Write<std::uint16_t>(369);  // buy menu length in bytes
    bufRef.Write<std::uint8_t>(2);     // unknown

    WriteBuyCategory(bufRef, buyMenu->GetPistols());
    WriteBuyCategory(bufRef, buyMenu->GetShotguns());
    WriteBuyCategory(bufRef, buyMenu->GetSmgs());
    WriteBuyCategory(bufRef, buyMenu->GetRifles());
    WriteBuyCategory(bufRef, buyMenu->GetSnipers());
    WriteBuyCategory(bufRef, buyMenu->GetMachineGuns());
    WriteBuyCategory(bufRef, buyMenu->GetMelees());
    WriteBuyCategory(bufRef, buyMenu->GetEquipment());

    res.Finish();

    return res;
}
