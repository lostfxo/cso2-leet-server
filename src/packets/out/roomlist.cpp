#include "packets/out/roomlist.hpp"

#include "util/number.hpp"

#include "room/room.hpp"

enum class OutRoomListType
{
    SendFullRoomList = 0
};

inline void WriteRoomListItem(DynamicBuffer& pktBuf, RoomPtr room)
{
    pktBuf.Write(util::FastNarrow<std::uint16_t>(room->GetId()));  // roomId
    pktBuf.Write<std::uint64_t>(0xFFFFFFFFFFFFFFFF);          // flags

    // flags & 0x1
    pktBuf.WriteString(room->GetSettings().GetRoomName());  // roomName
    // end flags & 0x1
    // flags & 0x2

    // TODO: this should be fine if it overflows right?
    pktBuf.Write(util::FastNarrow<uint8_t>(room->GetId()));  // roomNumber

    // end flags & 0x2
    // flags & 0x4
    pktBuf.Write<std::uint8_t>(
        room->IsPasswordProtected());  // passwordProtected
    // end flags & 0x4
    // flags & 0x8
    pktBuf.Write<std::uint16_t>(0);  // unk03
    // end flags & 0x8
    // flags & 0x10
    pktBuf.Write(
        std::uint8_t(room->GetSettings().GetGameMode()));  // gameModeId
    // end flags & 0x10
    // flags & 0x20
    pktBuf.Write<std::uint8_t>(room->GetSettings().GetMapId());  // mapId
    // end flags & 0x20
    // flags & 0x40
    pktBuf.Write(
        util::FastNarrow<std::uint8_t>(room->GetSlots().size()));  // numPlayers
    // end flags & 0x40
    // flags & 0x80
    pktBuf.Write<std::uint8_t>(
        room->GetSettings().GetMaxPlayers());  // maxPlayers
    // end flags & 0x80
    // flags & 0x100
    pktBuf.Write<std::uint8_t>(0);  // unk08
    // end flags & 0x100
    // flags & 0x200
    pktBuf.Write<std::uint32_t>(room->GetHostUserId());  // hostUserId
    pktBuf.WriteString(room->GetHostPlayerName());       // hostUserName
    pktBuf.Write<std::uint8_t>(0);                       // unk11
    // end flags & 0x200
    // flags & 0x400
    pktBuf.Write<std::uint8_t>(0);  // unk12
    // end flags & 0x400
    // flags & 0x800
    // maybe some ip? it looks like 61.164.61.215
    pktBuf.Write<std::uint32_t>(0xd73da43d);  // unk13
    // would this be some port? 40753 in decimal
    pktBuf.Write<std::uint16_t>(0x9f31);      // unk14
    pktBuf.Write<std::uint16_t>(0xb2b9);      // unk15
    pktBuf.Write<std::uint32_t>(0xd73da43d);  // unk16
    pktBuf.Write<std::uint16_t>(0x9f31);      // unk17
    pktBuf.Write<std::uint16_t>(0xb2b9);      // unk18
    pktBuf.Write<std::uint8_t>(5);            // unk19
    // end flags & 0x800
    // flags & 0x1000
    std::uint8_t unk20 = 0;  // unk20
    pktBuf.Write<std::uint8_t>(unk20);
    /* if (unk20 === 1); {
        // unknown values in here
        pktBuf.Write<std::uint32_t>(0); // unk2001
        pktBuf.Write<std::uint8_t>(0); // unk2002
        pktBuf.Write<std::uint32_t>(0); // unk2003
        pktBuf.Write<std::uint8_t>(0); // unk2004
    } */
    // end flags & 0x1000
    // flags & 0x2000
    pktBuf.Write<std::uint8_t>(5);  // unk21
    // end flags & 0x2000
    // flags & 0x4000
    pktBuf.Write(std::uint8_t(room->GetSettings().GetStatus()));  // roomStatus
    // end flags & 0x4000
    // flags & 0x8000
    pktBuf.Write<std::uint8_t>(
        room->GetSettings().AreBotsEnabled());  // areBotsEnabled
    // end flags & 0x8000
    // flags & 0x10000
    pktBuf.Write<std::uint8_t>(0);  // unk24
    // end flags & 0x10000
    // flags & 0x20000
    pktBuf.Write<std::uint16_t>(
        room->GetSettings().GetStartMoney());  // startMoney
    // end flags & 0x20000
    // flags & 0x40000
    pktBuf.Write<std::uint8_t>(0);  // unk26
    // end flags & 0x40000
    // flags & 0x80000
    pktBuf.Write<std::uint8_t>(0);  // unk27
    // end flags & 0x80000
    // flags & 0x100000
    pktBuf.Write<std::uint8_t>(0);  // unk28
    // end flags & 0x100000
    // flags & 0x200000
    pktBuf.Write<std::uint8_t>(1);  // unk29
    // end flags & 0x200000
    // flags & 0x400000
    pktBuf.Write<std::uint64_t>(0x5af6f7bf);  // unk30
    // end flags & 0x400000
    // flags & 0x800000
    pktBuf.Write<std::uint8_t>(room->GetSettings().GetWinLimit());  // winLimit
    pktBuf.Write<std::uint16_t>(
        room->GetSettings().GetKillLimit());  // killLimit
    pktBuf.Write<std::uint8_t>(
        room->GetSettings().GetForceCamera());  // forceCamera
    // end flags & 0x800000
    // flags & 0x1000000
    pktBuf.Write<std::uint8_t>(4);  // unk31
    // end flags & 0x1000000
    // flags & 0x2000000
    pktBuf.Write<std::uint8_t>(0);  // unk35
    // end flags & 0x2000000
    // flags & 0x4000000
    pktBuf.Write<std::uint8_t>(
        room->GetSettings().GetNextMapEnabled());  // nextMapEnabled
    // end flags & 0x4000000
    // flags & 0x8000000
    pktBuf.Write<std::uint8_t>(
        room->GetSettings().GetChangeTeams());  // changeTeams
    // end flags & 0x8000000
    // flags & 0x10000000
    pktBuf.Write<std::uint8_t>(0);  // areFlashesDisabled
    // end flags & 0x10000000
    // flags & 0x20000000
    pktBuf.Write<std::uint8_t>(0);  // canSpec
    // end flags & 0x20000000
    // flags & 0x40000000
    auto hostVipLevel = room->GetHostVipLevel();

    pktBuf.Write<std::uint8_t>(hostVipLevel > 0);  // isVipRoom
    pktBuf.Write<std::uint8_t>(hostVipLevel);      // vipRoomLevel
    // end flags & 0x40000000
    // flags & 0x80000000
    pktBuf.Write<std::uint8_t>(
        room->GetSettings().GetDifficulty());  // difficulty
                                               // end flags & 0x80000000
}

inline void WriteRoomListCollection(DynamicBuffer& pktBuf,
                                    const RoomList& rooms)
{
    pktBuf.Write(util::FastNarrow<std::uint16_t>(rooms.size()));

    for (const auto& room : rooms)
    {
        WriteRoomListItem(pktBuf, room);
    }
}

PacketBuilder OutRoomListPacket::SendRoomList(const RoomList& rooms)
{
    PacketBuilder res(PacketId::RoomList, 256);
    auto& bufRef = res.GetBuffer();

    bufRef.Write(std::uint8_t(OutRoomListType::SendFullRoomList));
    WriteRoomListCollection(bufRef, rooms);

    res.Finish();

    return res;
}
