#ifndef __ROOM_CHANNELGROUP_H_
#define __ROOM_CHANNELGROUP_H_

#include <array>
#include <string>

#include "channel.hpp"

template <std::size_t CHANNEL_COUNT>
class ChannelGroup
{
public:
    using ChannelsArray = std::array<Channel, CHANNEL_COUNT>;

    ChannelGroup(std::string&& groupName, int channelsCount)
        : m_GroupName(groupName)
    {
        for (int i = 0; i < channelsCount; i++)
        {
            this->m_Channels[i] = Channel(this->m_GroupName, i + 1);
        }
    }

private:
    std::string m_GroupName;
    ChannelsArray m_Channels;
};

#endif  // __ROOM_CHANNELGROUP_H_
