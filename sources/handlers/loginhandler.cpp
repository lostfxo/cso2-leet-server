#include "handlers.hpp"

#include <algorithm>

#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "packets/view.hpp"
#include "serveroptions.hpp"
#include "services/userservice.hpp"
#include "system/dialogbox.hpp"
#include "system/l10n.hpp"
#include "util/log.hpp"

#include "packets/in/login.hpp"
#include "packets/out/favorite.hpp"
#include "packets/out/inventory.hpp"
#include "packets/out/option.hpp"
#include "packets/out/unlock.hpp"
#include "packets/out/userinfo.hpp"
#include "packets/out/userstart.hpp"

awaitable<void> LoginSendInventory(std::uint32_t userId,
                                   ClientSessionPtr session)
{
    auto inventory = co_await g_UserService->GetInventory(userId);

    if (inventory == nullptr)
    {
        Log::Warning(
            "[LoginSendInventory] Failed to get inventory of user {}\n",
            userId);
        co_return;
    }

    auto cosmetics = co_await g_UserService->GetCosmetics(userId);

    if (cosmetics == nullptr)
    {
        Log::Warning(
            "[LoginSendInventory] Failed to get cosmetics of user {}\n",
            userId);
        co_return;
    }

    auto loadouts = co_await g_UserService->GetAllLoadouts(userId);

    bool anyLoadoutNull = false;

    for (const auto& loadout : loadouts)
    {
        if (loadout == nullptr)
        {
            anyLoadoutNull = true;
            break;
        }
    }

    if (anyLoadoutNull == true)
    {
        Log::Warning("[LoginSendInventory] Failed to get loadouts of user {}\n",
                     userId);
        co_return;
    }

    auto buyMenu = co_await g_UserService->GetBuyMenu(userId);

    if (buyMenu == nullptr)
    {
        Log::Warning("[LoginSendInventory] Failed to get buy menu of user {}\n",
                     userId);
        co_return;
    }

    session->Send(OutInventoryPacket::Create(inventory));
    session->Send(OutFavoritePacket::SetCosmetics(cosmetics));
    session->Send(OutFavoritePacket::SetAllLoadouts(loadouts));
    session->Send(OutOptionPacket::SetBuyMenu(buyMenu));
    session->Send(OutUnlockPacket::SetUnlocks({}, {}));
}

awaitable<void> OnLoginPacketRequest(PacketView& packet,
                                     ClientSessionPtr session)
{
    InLoginPacket loginPkt(packet);

    Log::Debug(
        "[OnLoginPacketRequest] session '{}' is trying to login as '{}'\n",
        session->GetUniqueId(), loginPkt.GameUsername);

    auto loggedUserId =
        co_await g_UserService->Login(loginPkt.GameUsername, loginPkt.Password);

    // clear the password from memory right away
    std::fill(loginPkt.Password.begin(), loginPkt.Password.end(), '\0');
    loginPkt.Password.clear();

    if (loggedUserId == cso2::INVALID_USER_ID)
    {
        SendDialogBox(session, l10n::LOGIN_BAD_USERNAME);
        co_return;
    }

    if (loggedUserId == cso2::UNAUTHORISED_USER_ID)
    {
        SendDialogBox(session, l10n::LOGIN_BAD_PASSWORD);
        co_return;
    }

    auto user = co_await g_UserService->GetUserById(loggedUserId);

    if (user == nullptr)
    {
        Log::Error(
            "[OnLoginPacketRequest] failed to get user's data with ID {}\n",
            loggedUserId);

        SendDialogBox(session, l10n::LOGIN_INVALID_USERINFO);
        co_return;
    }

    session->SetUser(user);

    Log::Debug(
        "[OnLoginPacketRequest] user '{}' logged in successfully (uid {})\n",
        loginPkt.GameUsername, session->GetUniqueId());

    session->Send(OutUserStartPacket::UserStart(
        loggedUserId, user->GetUserName(), user->GetPlayerName(),
        g_ServerOptions.PublicUdpPort));

    session->Send(OutUserInfoPacket::FullUpdate(user));

    co_await LoginSendInventory(loggedUserId, session);
}
