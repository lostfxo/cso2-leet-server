#include "packets/out/shop.hpp"

#include <array>

#include "packets/shopshared.hpp"
#include "util/number.hpp"

PacketBuilder OutShopPacket::Create(const std::vector<cso2::ShopItem>& items)
{
    PacketBuilder res(PacketId::Shop, 16384);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(ShopPacketType::Create));

    auto numItems = util::FastNarrow<std::uint16_t>(items.size());
    bufRef.Write(numItems);

    for (std::uint32_t curIndex = 0; const auto& item : items)
    {
        bufRef.Write(item.ItemId);
        bufRef.Write(std::uint8_t(item.Currency));

        auto numOptions = util::FastNarrow<std::uint8_t>(items.size());
        bufRef.Write(numOptions);

        for (const auto& option : item.PaymentOptions)
        {
            bufRef.Write(curIndex++);
            bufRef.Write(option.Quantity);
            bufRef.Write<std::uint64_t>(0);
            bufRef.Write<std::uint8_t>(0);
            bufRef.Write<std::uint16_t>(1);
            bufRef.Write(option.PreviousPrice);
            bufRef.Write(option.CurrentPrice);
            bufRef.Write(option.DiscountPercentage);
            bufRef.Write<std::uint32_t>(0);
            bufRef.Write<std::uint32_t>(0);
            bufRef.Write(option.Flags);
            bufRef.Write<std::uint8_t>(0);
            bufRef.Write<std::uint8_t>(1);
            bufRef.Write<std::uint8_t>(0);
            bufRef.Write(option.MileageReward);
            bufRef.Write<std::uint8_t>(0);
            bufRef.Write<std::uint8_t>(0);
        }
    }

    res.Finish();

    return res;
}
