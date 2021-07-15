#include "packets/out/serverlist.hpp"

#include "util/number.hpp"

#include "channel/channel.hpp"
#include "channel/channelgroup.hpp"
#include "cso2/definitions.hpp"

inline void WriteChannelInfo(DynamicBuffer& pktBuf, std::string_view name,
                             std::uint8_t id, cso2::ChannelTypes type,
                             cso2::ChannelStatuses status)
{
    pktBuf.Write<std::uint8_t>(id);
    pktBuf.WriteString(name);
    pktBuf.Write<std::uint16_t>(4);
    pktBuf.Write<std::uint16_t>(0x1F4);
    pktBuf.Write<std::uint8_t>(1);
    pktBuf.Write(std::uint8_t(type));
    pktBuf.Write(std::uint8_t(status));
}

inline void WriteServerItem(DynamicBuffer& pktBuf, std::string_view serverName,
                            const ChannelsArray& channels,
                            std::uint8_t serverIndex, std::uint8_t serverStatus,
                            std::uint8_t serverType)
{
    pktBuf.Write<std::uint8_t>(serverIndex);
    pktBuf.Write<std::uint8_t>(serverStatus);
    pktBuf.Write<std::uint8_t>(serverType);
    pktBuf.WriteString(serverName);
    pktBuf.Write(util::FastNarrow<std::uint8_t>(channels.size()));

    for (const auto& channel : channels)
    {
        WriteChannelInfo(pktBuf, channel->GetName(), channel->GetIndex(),
                         channel->GetType(), channel->GetStatus());
    }
}

PacketBuilder OutServerListPacket::ServerList(const ChannelGroupsArray groups)
{
    PacketBuilder res(PacketId::ServerList, 256);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(util::FastNarrow<std::uint8_t>(groups.size()));

    for (const auto& group : groups)
    {
        WriteServerItem(bufRef, group->GetGroupName(), group->GetChannels(), 1,
                        1, 1);
    }

    res.Finish();

    return res;
}
