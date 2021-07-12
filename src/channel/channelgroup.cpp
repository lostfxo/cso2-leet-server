#include "channel/channelgroup.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "channel/channel.hpp"

ChannelGroup::ChannelGroup(std::string&& groupName, std::size_t index,
                           std::size_t channelsCount)
    : m_Index(index), m_GroupName(std::move(groupName))
{
    if (channelsCount > std::numeric_limits<std::uint8_t>::max())
    {
        throw std::length_error(
            "A channel group may not have more than 255 channels");
    }

    this->m_Channels.reserve(channelsCount);

    for (std::size_t i = 0; i < channelsCount; i++)
    {
        this->m_Channels.push_back(
            std::make_shared<Channel>(this->m_GroupName, i + 1, channelsCount));
    }
}
ChannelPtr ChannelGroup::GetChannelById(std::size_t channelId) const noexcept
{
    return *std::find_if(this->m_Channels.begin(), this->m_Channels.end(),
                         [channelId](const ChannelPtr channel) {
                             return channel->GetIndex() == channelId;
                         });
}
