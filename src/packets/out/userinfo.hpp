#ifndef __OUT_PKT_USERINFO_
#define __OUT_PKT_USERINFO_

#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"
#include "packets/builder.hpp"

class OutUserInfoPacket
{
public:
    static PacketBuilder FullUpdate(cso2::UserPtr user);

    static PacketBuilder UpdateAvatar(cso2::UserPtr user);
    static PacketBuilder UpdateSignature(cso2::UserPtr user);
    static PacketBuilder UpdateTitle(cso2::UserPtr user);

    static PacketBuilder UpdateCampaignFlags(cso2::UserPtr user);
    static PacketBuilder UpdateKillStats(cso2::UserPtr user);
};

#endif  // __OUT_PKT_USERINFO_
