#include "handlers.hpp"

#include "clientsession.hpp"
#include "packets/view.hpp"
#include "util/log.hpp"

#include "packets/in/version.hpp"
#include "packets/out/version.hpp"

void OnVersionPacketRequest(PacketView& packet, ClientSessionPtr session)
{
    InVersionPacket ver(packet);
    Log::Debug("[OnVersionPacketRequest] '{}' sent client hash '{}'\n",
               session->GetUniqueId(), ver.ClientHash);

    auto res =
        OutVersionPacket::Version("6246015df9a7d1f7311f888e7e861f18", false);
    session->Send(std::move(res));
}
