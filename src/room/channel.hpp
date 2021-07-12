#ifndef __ROOM_CHANNEL_H_
#define __ROOM_CHANNEL_H_

#include <string>
#include <string_view>

#include <fmt/format.h>

class Channel
{
public:
    Channel(std::string_view groupName, int channelIndex)
        : m_Index(channelIndex)
    {
        this->m_Name = fmt::format("{}-{}", groupName, channelIndex);
    }

private:
    int m_Index;
    std::string m_Name;
};

#endif  // __ROOM_CHANNEL_H_
