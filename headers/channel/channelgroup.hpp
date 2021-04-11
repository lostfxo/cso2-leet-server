#ifndef __ROOM_CHANNELGROUP_H_
#define __ROOM_CHANNELGROUP_H_

#include <string>
#include <string_view>
#include <vector>

#include "channel/shared.hpp"

class ChannelGroup
{
public:
    ChannelGroup(std::string&& groupName, std::size_t index, std::size_t channelsCount);

    [[nodiscard]] inline std::size_t GetIndex() const noexcept
    {
        return this->m_Index;
    }

    [[nodiscard]] inline std::string_view GetGroupName() const noexcept
    {
        return this->m_GroupName;
    }

    [[nodiscard]] inline const ChannelsArray& GetChannels() const noexcept
    {
        return this->m_Channels;
    }

    [[nodiscard]] ChannelPtr GetChannelById(std::size_t channelId) const noexcept;

private:
    std::size_t m_Index;
    std::string m_GroupName;
    ChannelsArray m_Channels;
};

#endif  // __ROOM_CHANNELGROUP_H_
