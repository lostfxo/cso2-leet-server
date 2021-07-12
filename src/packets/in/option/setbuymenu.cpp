#include "packets/in/option/setbuymenu.hpp"

#include <stdexcept>

#include "cso2/shared.hpp"
#include "packets/view.hpp"

inline cso2::BuyCategory ReadBuyCategory(PacketView& packet)
{
    auto numCategoryItems = packet.Read<std::uint8_t>();

    if (numCategoryItems != cso2::MAX_BUY_CATEGORY_ITEMS)
    {
        throw std::runtime_error(
            "The buy category's items ammount is incorrect");
    }

    cso2::BuyCategory res;

    for (std::size_t i = 0; i < res.max_size(); i++)
    {
        auto curIndex = packet.Read<std::uint8_t>();

        if (curIndex != i)
        {
            throw std::runtime_error("The current sub buy menu index is "
                                     "different from the expected.");
        }

        res[i] = packet.Read<std::uint32_t>();
    }

    return res;
}

InOptionPacketSetBuyMenu::InOptionPacketSetBuyMenu(PacketView& packet)
{
    this->MenuByteLength = packet.Read<std::uint16_t>();
    this->unk00 = packet.Read<std::uint8_t>();

    this->BuyMenu.Pistols = ReadBuyCategory(packet);
    this->BuyMenu.Shotguns = ReadBuyCategory(packet);
    this->BuyMenu.Smgs = ReadBuyCategory(packet);
    this->BuyMenu.Rifles = ReadBuyCategory(packet);
    this->BuyMenu.Snipers = ReadBuyCategory(packet);
    this->BuyMenu.MachineGuns = ReadBuyCategory(packet);
    this->BuyMenu.Melees = ReadBuyCategory(packet);
    this->BuyMenu.Equipment = ReadBuyCategory(packet);
}
