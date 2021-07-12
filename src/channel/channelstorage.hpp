#ifndef __CHANNEL_CHANNELSTORAGE_H_
#define __CHANNEL_CHANNELSTORAGE_H_

#include "channel/channelgroup.hpp"

class ChannelStorage
{
public:
    ChannelStorage();

    [[nodiscard]] inline const ChannelGroupsArray& GetGroups() const noexcept
    {
        return this->m_Groups;
    }

    [[nodiscard]] ChannelGroupPtr GetGroupByIndex(std::size_t index) const noexcept;

private:
    ChannelGroupsArray m_Groups;
};

extern ChannelStorage g_Channels;

#endif  // __CHANNEL_CHANNELSTORAGE_H_
