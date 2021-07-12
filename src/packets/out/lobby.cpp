#include "packets/out/lobby.hpp"

enum class OutLobbyType : std::uint8_t
{
    JoinRoomList = 1,
    UpdateUserInfo = 2
};

PacketBuilder OutLobbyPacket::JoinRoomList()
{
    PacketBuilder res(PacketId::Lobby, 16);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(OutLobbyType::JoinRoomList));

    bufRef.Write<std::uint8_t>(0);  // unk00
    bufRef.Write<std::uint8_t>(2);  // unk01
    bufRef.Write<std::uint8_t>(4);  // unk02

    res.Finish();

    return res;
}
