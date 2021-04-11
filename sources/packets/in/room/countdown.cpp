#include "packets/in/room/countdown.hpp"

#include "packets/view.hpp"

enum class RoomCountdownType : std::uint8_t
{
    InProgress = 0,
    Stop = 1
};

InRoomPacketCountdownRequest::InRoomPacketCountdownRequest(PacketView& packet)
{
    this->CountingDown = RoomCountdownType(packet.Read<std::uint8_t>()) ==
                         RoomCountdownType::InProgress;

    if (this->CountingDown == true)
    {
        this->CountNum = packet.Read<std::uint8_t>();
    }
}
