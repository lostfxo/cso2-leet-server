#include "channel/channel.hpp"

#include <fmt/format.h>
#include <algorithm>

#include "clientsession.hpp"
#include "cso2/user.hpp"
#include "room/room.hpp"

namespace ranges = std::ranges;

Channel::Channel(std::string_view groupName, std::uint8_t channelIndex,
                 std::size_t totalChannelsCount)
    : m_Type(cso2::ChannelTypes::Free), m_Status(cso2::ChannelStatuses::Normal),
      m_NextRoomId(1), m_Index(channelIndex)
{
    this->m_Name =
        fmt::format("{} {}-{}", groupName, totalChannelsCount, channelIndex);
}

void Channel::AddRoom(RoomPtr newRoom)
{
    this->m_Rooms.push_front(newRoom);
    newRoom->AddEmptyListener(
        [this](RoomPtr room) { this->OnRoomEmptied(room); });
}

RoomPtr Channel::FindRoomById(std::uint32_t targetId) const
{
    return *ranges::find_if(this->m_Rooms, [targetId](const auto& r) {
        return r->GetId() == targetId;
    });
}

void Channel::AddSessionToChannel(ClientSessionPtr session)
{
    this->m_ConnectedSessions.push_front(session);
    session->SetCurChannel(shared_from_this());
}

void Channel::RemoveSessionFromChannel(ClientSessionPtr session)
{
    auto curRoom = session->GetCurRoom();

    if (curRoom != nullptr)
    {
        curRoom->RemoveSlotById(session->GetUser()->GetId());
        session->SetCurRoom(nullptr);
    }

    this->m_ConnectedSessions.remove(session);
    session->SetCurChannel(nullptr);
}

void Channel::OnRoomEmptied(RoomPtr room)
{
    this->m_Rooms.remove(room);
    Log::Debug("[Channel::OnRoomEmptied] channel {} deleted room id {}\n",
               this->m_Name, room->GetId());
}
