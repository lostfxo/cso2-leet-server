#include "handlers.hpp"

#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "packets/view.hpp"
#include "services/userservice.hpp"
#include "util/log.hpp"

#include "packets/in/favorite.hpp"
#include "packets/in/favorite/setcosmetics.hpp"
#include "packets/in/favorite/setloadout.hpp"

awaitable<void> OnFavoriteSetCosmeticRequest(PacketView& packet,
                                             ClientSessionPtr session)
{
    auto cosmeticPkt = InFavoritePacketSetCosmetics(packet);

    bool res = co_await g_UserService->SetCosmeticSlot(
        session->GetUser()->GetId(), cosmeticPkt.Slot, cosmeticPkt.ItemId);

    if (res == false)
    {
        Log::Warning(
            "[OnFavoriteSetCosmeticRequest] failed to set cosmetic for user "
            "'{}'. slot: {} newItem: {}\n",
            session->GetUser()->GetId(), cosmeticPkt.Slot, cosmeticPkt.ItemId);
    }
}

awaitable<void> OnFavoriteSetLoadoutRequest(PacketView& packet,
                                            ClientSessionPtr session)
{
    auto loadoutPkt = InFavoritePacketSetLoadout(packet);

    bool res = co_await g_UserService->SetLoadoutWeapon(
        session->GetUser()->GetId(), loadoutPkt.LoadoutNum,
        loadoutPkt.WeaponSlot, loadoutPkt.ItemId);

    if (res == false)
    {
        Log::Warning(
            "[OnFavoriteSetLoadoutRequest] failed to set loadout for user "
            "'{}'. loadout index: {} slot: {} newItem: {}\n",
            session->GetUser()->GetId(), loadoutPkt.LoadoutNum,
            loadoutPkt.WeaponSlot, loadoutPkt.ItemId);
    }
}

awaitable<void> OnFavoritePacketRequest(PacketView& packet,
                                        ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnFavoritePacketRequest] conn '{}' sent favorite "
                     "packet without session\n",
                     session->GetUniqueId());
        co_return;
    }

    auto favPkt = InFavoritePacket(packet);

    switch (favPkt.Type)
    {
        case FavoritePacketType::SetLoadout:
            co_await OnFavoriteSetLoadoutRequest(packet, session);
            break;
        case FavoritePacketType::SetCosmetics:
            co_await OnFavoriteSetCosmeticRequest(packet, session);
            break;
        default:
            Log::Warning(
                "[OnFavoritePacketRequest] user {} sent unknown favorite "
                "packet sub type {}\n",
                session->GetUser()->GetId(), favPkt.Type);
    }
}
