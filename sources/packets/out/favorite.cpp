#include "packets/out/favorite.hpp"

#include "cso2/cosmetics.hpp"
#include "cso2/loadout.hpp"
#include "packets/favoriteshared.hpp"

inline void WriteCosmetic(DynamicBuffer& buf, std::uint32_t id,
                          std::uint8_t index)
{
    buf.Write(index);
    buf.Write(id);
}

inline void WriteLoadoutItem(DynamicBuffer& buf, std::uint32_t id,
                             std::uint8_t loadoutNum, std::uint8_t index)
{
    buf.Write(loadoutNum);
    buf.Write(index);
    buf.Write(id);
}

PacketBuilder OutFavoritePacket::SetCosmetics(cso2::CosmeticsPtr cosmetics)
{
    PacketBuilder res(PacketId::Favorite, 64);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(FavoritePacketType::SetCosmetics));

    bufRef.Write<std::uint8_t>(10);  // the ammount of items being sent

    std::uint8_t nextItemIndex = 0;

    WriteCosmetic(bufRef, cosmetics->GetCtItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetTerItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetHeadItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetGloveItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetBackItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetStepsItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetCardItem(), nextItemIndex++);
    WriteCosmetic(bufRef, cosmetics->GetSprayItem(), nextItemIndex++);
    WriteCosmetic(bufRef, 0, nextItemIndex++);  // unk03
    WriteCosmetic(bufRef, 0, nextItemIndex++);  // unk04

    res.Finish();

    return res;
}

PacketBuilder OutFavoritePacket::SetAllLoadouts(
    const cso2::LoadoutsArray& loadouts)
{
    PacketBuilder res(PacketId::Favorite, 320);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(FavoritePacketType::SetLoadout));
    bufRef.Write<std::uint8_t>(cso2::MAX_LOADOUTS_NUM *
                               cso2::MAX_LOADOUT_ITEMS_NUM);

    for (auto loadout : loadouts)
    {
        std::uint8_t nextItemIndex = 0;
        auto loadoutNum = loadout->GetLoadoutNum();

        WriteLoadoutItem(bufRef, loadout->GetPrimaryWeapon(), loadoutNum,
                         nextItemIndex++);
        WriteLoadoutItem(bufRef, loadout->GetSecondaryWeapon(), loadoutNum,
                         nextItemIndex++);
        WriteLoadoutItem(bufRef, loadout->GetMelee(), loadoutNum,
                         nextItemIndex++);
        WriteLoadoutItem(bufRef, loadout->GetHeGrenade(), loadoutNum,
                         nextItemIndex++);
        WriteLoadoutItem(bufRef, loadout->GetSmoke(), loadoutNum,
                         nextItemIndex++);
        WriteLoadoutItem(bufRef, loadout->GetFlash(), loadoutNum,
                         nextItemIndex++);

        for (; nextItemIndex < cso2::MAX_LOADOUT_ITEMS_NUM; nextItemIndex++)
        {
            WriteLoadoutItem(bufRef, 0, loadoutNum, nextItemIndex);
        }
    }

    res.Finish();

    return res;
}
