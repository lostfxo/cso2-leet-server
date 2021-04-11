#ifndef __ROOM_CHANNEL_H_
#define __ROOM_CHANNEL_H_

#include <forward_list>
#include <string_view>

#include "channel/shared.hpp"
#include "room/shared.hpp"

class Channel : public std::enable_shared_from_this<Channel>
{
public:
    Channel(std::string_view groupName, std::uint8_t channelIndex,
            std::size_t totalChannelsCount);

    [[nodiscard]] inline std::uint8_t GetIndex() const noexcept
    {
        return this->m_Index;
    }

    [[nodiscard]] inline std::string_view GetName() const noexcept
    {
        return this->m_Name;
    }

    [[nodiscard]] inline cso2::ChannelTypes GetType() const noexcept
    {
        return this->m_Type;
    }

    [[nodiscard]] inline cso2::ChannelStatuses GetStatus() const noexcept
    {
        return this->m_Status;
    }

    [[nodiscard]] inline const auto& GetSessions() const noexcept
    {
        return this->m_ConnectedSessions;
    }

    [[nodiscard]] inline const auto& GetRooms() const noexcept
    {
        return this->m_Rooms;
    }

    void AddRoom(RoomPtr newRoom);
    RoomPtr FindRoomById(std::uint32_t targetId) const;

    void AddSessionToChannel(ClientSessionPtr session);
    void RemoveSessionFromChannel(ClientSessionPtr session);

    [[nodiscard]] inline std::uint16_t GetNextRoomId() noexcept
    {
        return this->m_NextRoomId++;
    }

protected:
    void OnRoomEmptied(RoomPtr room);

private:
    std::string m_Name;
    cso2::ChannelTypes m_Type;
    cso2::ChannelStatuses m_Status;

    std::forward_list<ClientSessionPtr> m_ConnectedSessions;
    RoomList m_Rooms;

    std::uint16_t m_NextRoomId;

    std::uint8_t m_Index;
};

#endif  // __ROOM_CHANNEL_H_
