#include "channel/channelstorage.hpp"

#include <algorithm>
#include <stdexcept>

namespace ranges = std::ranges;
using namespace std::string_literals;

ChannelStorage g_Channels;

ChannelStorage::ChannelStorage()
{
    this->m_Groups = { std::make_shared<ChannelGroup>("Test server"s, 1, 1) };

    if (this->m_Groups.size() >= std::numeric_limits<std::uint8_t>::max())
    {
        throw std::length_error(
            "There cannot be more than 255 channel groups in a server");
    }
}

ChannelGroupPtr ChannelStorage::GetGroupByIndex(std::size_t index) const noexcept
{
    return *ranges::find_if(this->m_Groups,
                            [index](const ChannelGroupPtr group) {
                                return group->GetIndex() == index;
                            });
}
