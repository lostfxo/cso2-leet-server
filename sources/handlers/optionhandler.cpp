#include "handlers.hpp"

#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "packets/view.hpp"
#include "services/userservice.hpp"
#include "util/log.hpp"

#include "packets/in/option.hpp"
#include "packets/in/option/setbuymenu.hpp"
#include "packets/optionshared.hpp"

awaitable<void> OnSetBuyMenuRequest(PacketView& packet,
                                    ClientSessionPtr session)
{
    auto buymenuPkt = InOptionPacketSetBuyMenu(packet);

    bool res = co_await g_UserService->SetBuyMenu(session->GetUser()->GetId(),
                                                  buymenuPkt.BuyMenu);

    if (res == false)
    {
        Log::Warning(
            "[OnSetBuyMenuRequest] failed to set buy menu for user '{}'\n",
            session->GetUser()->GetId());
    }
}
awaitable<void> OnOptionPacketRequest(PacketView& packet,
                                      ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnOptionPacketRequest] conn '{}' sent favorite "
                     "packet without session\n",
                     session->GetUniqueId());
        co_return;
    }

    auto optPkt = InOptionPacket(packet);

    switch (optPkt.Type)
    {
        case OptionPacketType::SetBuyMenu:
            co_await OnSetBuyMenuRequest(packet, session);
            break;
        default:
            Log::Warning(
                "[OnOptionPacketRequest] user {} sent unknown favorite "
                "packet sub type {}\n",
                session->GetUser()->GetId(), optPkt.Type);
    }
}
