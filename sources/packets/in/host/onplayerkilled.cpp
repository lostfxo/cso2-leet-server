#include "packets/in/host/onplayerkilled.hpp"

#include "packets/view.hpp"

InHostPacketOnPlayerKilled::InHostPacketOnPlayerKilled(PacketView& packet)
{
    this->DamageInfo.Flags = cso2::KillFlags(packet.Read<std::uint32_t>());

    this->DamageInfo.Attacker.UserId = packet.Read<std::uint32_t>();
    this->DamageInfo.Attacker.WeaponId = packet.Read<std::uint32_t>();
    this->DamageInfo.Attacker.Team = cso2::TeamNum(packet.Read<std::uint8_t>());
    this->DamageInfo.Attacker.Type = cso2::ClientType(packet.Read<std::uint8_t>());
    this->DamageInfo.Attacker.Character =
        cso2::CharacterType(packet.Read<std::uint8_t>());
    this->DamageInfo.Attacker.CharacterClass = packet.Read<std::uint32_t>();

    this->DamageInfo.someVictimSpecialFlags = packet.Read<std::uint32_t>();

    this->DamageInfo.Victim.UserId = packet.Read<std::uint32_t>();
    this->DamageInfo.Victim.WeaponId = packet.Read<std::uint32_t>();
    this->DamageInfo.Victim.Team = cso2::TeamNum(packet.Read<std::uint8_t>());
    this->DamageInfo.Victim.Type = cso2::ClientType(packet.Read<std::uint8_t>());
    this->DamageInfo.Victim.Character =
        cso2::CharacterType(packet.Read<std::uint8_t>());
    this->DamageInfo.Victim.CharacterClass = packet.Read<std::uint32_t>();

    this->DamageInfo.AttackerPos = IntVector(
        packet.Read<int32_t>(), packet.Read<int32_t>(), packet.Read<int32_t>());
    this->DamageInfo.VictimPos = IntVector(
        packet.Read<int32_t>(), packet.Read<int32_t>(), packet.Read<int32_t>());

    auto unkArrayCount = packet.Read<std::uint16_t>();
    this->DamageInfo.unkArray.resize(unkArrayCount);

    for (std::size_t i = 0; i < unkArrayCount; i++)
    {
        this->DamageInfo.unkArray.emplace_back(
            packet.Read<std::uint8_t>(), packet.Read<std::uint32_t>(),
            packet.Read<std::uint32_t>(), packet.Read<std::uint8_t>());
    }
}
