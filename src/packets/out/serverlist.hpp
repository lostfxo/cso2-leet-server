#ifndef __OUT_PKT_SERVERLIST_
#define __OUT_PKT_SERVERLIST_

#include "channel/shared.hpp"
#include "packets/builder.hpp"

class OutServerListPacket
{
public:
    static PacketBuilder ServerList(const ChannelGroupsArray groups);
};

#endif  // __OUT_PKT_SERVERLIST_
