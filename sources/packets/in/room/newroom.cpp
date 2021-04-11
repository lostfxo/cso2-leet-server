#include "packets/in/room/newroom.hpp"

#include "packets/view.hpp"

InRoomPacketNewRequest::InRoomPacketNewRequest(PacketView& packet)
{
    this->Options.RoomName = packet.ReadString();
    this->Options.unk00 = packet.Read<std::uint16_t>();
    this->Options.unk01 = packet.Read<std::uint8_t>();
    this->Options.GameModeId =
        static_cast<cso2::GameMode>(packet.Read<std::uint8_t>());
    this->Options.MapId = packet.Read<std::uint8_t>();
    this->Options.WinLimit = packet.Read<std::uint8_t>();
    this->Options.KillLimit = packet.Read<std::uint16_t>();
    this->Options.unk02 = packet.Read<std::uint8_t>();
    this->Options.unk03 = packet.Read<std::uint8_t>();
    this->Options.unk04 = packet.Read<std::uint8_t>();
    this->Options.Password = packet.ReadString();
    this->Options.unk06 = packet.Read<std::uint8_t>();
    this->Options.unk07 = packet.Read<std::uint8_t>();
    this->Options.unk08 = packet.Read<std::uint8_t>();
    this->Options.unk09 = packet.Read<std::uint8_t>();
    this->Options.unk10 = packet.Read<std::uint8_t>();
    this->Options.unk11 = packet.Read<std::uint32_t>();
}
