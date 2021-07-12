#include "handlers.hpp"

#include <algorithm>

#include "activesessions.hpp"
#include "clientsession.hpp"
#include "cso2/inventory.hpp"
#include "cso2/user.hpp"
#include "packets/view.hpp"
#include "room/room.hpp"
#include "services/userservice.hpp"
#include "util/log.hpp"

#include "packets/in/host.hpp"
#include "packets/in/host/changeteam.hpp"
#include "packets/in/host/itemused.hpp"
#include "packets/in/host/onplayerbuyitem.hpp"
#include "packets/in/host/onplayerkilled.hpp"
#include "packets/in/host/onplayerscored.hpp"
#include "packets/in/host/onplayerspawn.hpp"
#include "packets/in/host/onroundend.hpp"
#include "packets/in/host/setinvobj.hpp"
#include "packets/out/host.hpp"

awaitable<void> OnHostGameEndRequest(ClientSessionPtr session)
{
    auto curRoom = session->GetCurRoom();
    Log::Debug("[OnHostGameEndRequest] ending match in room {}\n",
               curRoom->GetId());
    co_await curRoom->EndGame();
}

awaitable<void> OnHostSetInventoryRequest(PacketView& packet,
                                          ClientSessionPtr session)
{
    auto reqPkt = InHostPacketSetInvObject(packet);
    auto curRoom = session->GetCurRoom();

    if (curRoom->IsUserInRoom(reqPkt.UserId) == false)
    {
        Log::Warning(
            "[OnHostSetInventoryRequest] target user {} is not in the room\n",
            reqPkt.UserId);
        co_return;
    }

    auto targetInv = co_await g_UserService->GetInventory(reqPkt.UserId);

    if (targetInv == nullptr)
    {
        Log::Warning(
            "[OnHostSetInventoryRequest] failed to get user {}'s inventory\n",
            reqPkt.UserId);
        co_return;
    }

    Log::Debug(
        "[OnHostSetInventoryRequest] sending user {} inventory to host {}\n",
        reqPkt.UserId, session->GetUser()->GetId());

    auto invPkt =
        OutHostPacket::SetInventory(reqPkt.UserId, targetInv->GetItems());
    session->Send(std::move(invPkt));
}

awaitable<void> OnHostSetLoadoutRequest(PacketView& packet,
                                        ClientSessionPtr session)
{
    auto reqPkt = InHostPacketSetInvObject(packet);
    auto curRoom = session->GetCurRoom();

    if (curRoom->IsUserInRoom(reqPkt.UserId) == false)
    {
        Log::Warning(
            "[OnHostSetLoadoutRequest] target user {} is not in the room\n",
            reqPkt.UserId);
        co_return;
    }

    auto targetCosmetics = co_await g_UserService->GetCosmetics(reqPkt.UserId);

    if (targetCosmetics == nullptr)
    {
        Log::Warning(
            "[OnHostSetLoadoutRequest] failed to get user {}'s cosmetics\n",
            reqPkt.UserId);
        co_return;
    }

    auto targetLoadouts = co_await g_UserService->GetAllLoadouts(reqPkt.UserId);

    if (std::find_if(targetLoadouts.begin(), targetLoadouts.end(),
                     [](const auto& item) { return item == nullptr; }) !=
        targetLoadouts.end())
    {
        Log::Warning(
            "[OnHostSetLoadoutRequest] failed to get user {}'s loadouts\n",
            reqPkt.UserId);
        co_return;
    }

    Log::Debug("[OnHostSetLoadoutRequest] sending user {} loadout to host {}\n",
               reqPkt.UserId, session->GetUser()->GetId());

    auto invPkt = OutHostPacket::SetLoadout(reqPkt.UserId, targetCosmetics,
                                            targetLoadouts);
    session->Send(std::move(invPkt));
}

awaitable<void> OnHostSetBuyMenuRequest(PacketView& packet,
                                        ClientSessionPtr session)
{
    auto reqPkt = InHostPacketSetInvObject(packet);
    auto curRoom = session->GetCurRoom();

    if (curRoom->IsUserInRoom(reqPkt.UserId) == false)
    {
        Log::Warning(
            "[OnHostSetBuyMenuRequest] target user {} is not in the room\n",
            reqPkt.UserId);
        co_return;
    }

    auto targetBuyMenu = co_await g_UserService->GetBuyMenu(reqPkt.UserId);

    if (targetBuyMenu == nullptr)
    {
        Log::Warning(
            "[OnHostSetBuyMenuRequest] failed to get user {}'s buy menu\n",
            reqPkt.UserId);
        co_return;
    }

    Log::Debug(
        "[OnHostSetBuyMenuRequest] sending user {} inventory to host {}\n",
        reqPkt.UserId, session->GetUser()->GetId());

    auto buyMenuPkt = OutHostPacket::SetBuyMenu(reqPkt.UserId, *targetBuyMenu);
    session->Send(std::move(buyMenuPkt));
}

void OnHostChangeTeamRequest(PacketView& packet, ClientSessionPtr session)
{
    auto reqPkt = InHostPacketChangeTeam(packet);
    auto curRoom = session->GetCurRoom();

    if (curRoom->IsUserInRoom(reqPkt.UserId) == false)
    {
        Log::Warning(
            "[OnHostChangeTeamRequest] target user {} is not in the room\n",
            reqPkt.UserId);
        return;
    }

    if (cso2::IsInAnyTeam(reqPkt.NewTeam) == false)
    {
        Log::Warning("[OnHostChangeTeamRequest] target user {}'s team {} is "
                     "not valid\n",
                     reqPkt.UserId, reqPkt.NewTeam);
        return;
    }

    Log::Debug("[OnHostChangeTeamRequest] sending team change to {} of user "
               "{} to host {}\n",
               reqPkt.NewTeam, reqPkt.UserId, session->GetUser()->GetId());

    curRoom->UpdateUserTeam(reqPkt.UserId, reqPkt.NewTeam);
}

void OnPlayerItemUseUsedRequest(PacketView& packet, ClientSessionPtr session)
{
    auto reqPkt = InHostPacketItemUsed(packet);
    auto curRoom = session->GetCurRoom();

    if (curRoom->IsUserInRoom(reqPkt.UserId) == false)
    {
        Log::Warning(
            "[OnPlayerItemUseUsedRequest] target user {} is not in the room\n",
            reqPkt.UserId);
        return;
    }

    // TODO: fetch current ammount of uses left in the inventory and use it as
    // the third parameter of the packet
    auto usePkt = OutHostPacket::ItemUsed(reqPkt.UserId, reqPkt.ItemId, 1);
    session->Send(std::move(usePkt));
}

void OnHostPlayerScoredEvent(PacketView& packet, ClientSessionPtr session)
{
    auto pointPkt = InHostPacketOnPlayerScored(packet);
    Log::Debug(
        "[OnHostPlayerScoredEvent] {} scored {} frags. total frags: {}\n",
        pointPkt.ScorerUserId, pointPkt.NewFragsCount, pointPkt.TotalFrags);
}

void OnHostPlayerKilledEvent(PacketView& packet, ClientSessionPtr session)
{
    auto killPkt = InHostPacketOnPlayerKilled(packet);

    bool headshot =
        killPkt.DamageInfo.Flags & cso2::KillFlags::KilledByHeadshot;

    Log::Debug("[OnHostPlayerKilledEvent] {} killed {} with {}. headshot: {}, "
               "kill flags: {}\n",
               killPkt.DamageInfo.Attacker.UserId,
               killPkt.DamageInfo.Victim.UserId,
               killPkt.DamageInfo.Attacker.WeaponId, headshot,
               killPkt.DamageInfo.Flags);

    auto curRoom = session->GetCurRoom();
    Assert(curRoom != nullptr);

    curRoom->OnPlayerKilled(killPkt.DamageInfo.Attacker.UserId,
                            killPkt.DamageInfo.Victim.UserId, headshot);
}

void OnHostRoundEndEvent(PacketView& packet, ClientSessionPtr session)
{
    auto roundPkt = InHostPacketOnRoundEnd(packet);

    Log::Debug("[OnHostRoundEndEvent] team {} won a round. total ct wins: {}, "
               "total ter wins {}\n",
               roundPkt.WinnerTeam, roundPkt.TotalCtWins,
               roundPkt.TotalTerWins);

    auto curRoom = session->GetCurRoom();
    Assert(curRoom != nullptr);

    curRoom->OnRoundEnd(roundPkt.WinnerTeam);
}

void OnHostPlayerBuyItem(PacketView& packet, ClientSessionPtr session)
{
    auto buyPkt = InHostPacketOnPlayerBuyItem(packet);
    Log::Debug("[OnHostPlayerBuyItem] user {} bought item {}\n",
               buyPkt.BuyerUserId, buyPkt.ItemsBought[0]);
}

void OnHostPlayerSpawn(PacketView& packet, ClientSessionPtr session)
{
    auto spawnPkt = InHostPacketOnPlayerSpawn(packet);
    Log::Debug(
        "[OnHostPlayerSpawn] user {} spawned at {} {} {} (is spectating: {})\n",
        spawnPkt.PlayerUserId, spawnPkt.SpawnPoint.x, spawnPkt.SpawnPoint.y,
        spawnPkt.SpawnPoint.z, spawnPkt.Spectating);
}

awaitable<void> OnHostPacketRequest(PacketView& packet,
                                    ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnHostPacketRequest] conn '{}' sent host packet "
                     "without session\n",
                     session->GetUniqueId());
        co_return;
    }

    auto curRoom = session->GetCurRoom();

    if (curRoom == nullptr)
    {
        Log::Warning("[OnHostPacketRequest] conn '{}' sent host packet "
                     "without being in a room\n",
                     session->GetUniqueId());
        co_return;
    }

    if (curRoom->GetHostUserId() != session->GetUser()->GetId())
    {
        Log::Warning("[OnHostPacketRequest] conn '{}' sent host packet "
                     "without being the current room's host\n",
                     session->GetUniqueId());
        co_return;
    }

    auto hostPkt = InHostPacket(packet);

    switch (hostPkt.Type)
    {
        case InHostPacketType::OnGameEnd:
            co_await OnHostGameEndRequest(session);
            break;
        case InHostPacketType::SetInventory:
            co_await OnHostSetInventoryRequest(packet, session);
            break;
        case InHostPacketType::SetLoadout:
            co_await OnHostSetLoadoutRequest(packet, session);
            break;
        case InHostPacketType::SetBuyMenu:
            co_await OnHostSetBuyMenuRequest(packet, session);
            break;
        case InHostPacketType::ChangeTeam:
            OnHostChangeTeamRequest(packet, session);
            break;
        case InHostPacketType::OnPlayerItemUse:
            OnPlayerItemUseUsedRequest(packet, session);
            break;
        case InHostPacketType::OnPlayerScored:
            OnHostPlayerScoredEvent(packet, session);
            break;
        case InHostPacketType::OnPlayerKilled:
            OnHostPlayerKilledEvent(packet, session);
            break;
        case InHostPacketType::OnRoundEnd:
            OnHostRoundEndEvent(packet, session);
            break;
        case InHostPacketType::OnPlayerBuyItem:
            OnHostPlayerBuyItem(packet, session);
            break;
        case InHostPacketType::OnPlayerSpawn:
            OnHostPlayerSpawn(packet, session);
            break;
    }

    Log::Debug("[OnHostPacketRequest] user {} sent unknown host packet id {}\n",
               session->GetUser()->GetId(), hostPkt.Type);
}
