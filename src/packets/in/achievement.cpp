#include "packets/in/achievement.hpp"

#include "packets/view.hpp"

InAchievementPacket::InAchievementPacket(PacketView& packet)
{
    this->Type = AchievementPacketType(packet.Read<std::uint8_t>());
}
