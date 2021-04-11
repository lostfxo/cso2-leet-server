#include "packets/out/userstart.hpp"

PacketBuilder OutUserStartPacket::UserStart(std::uint32_t userId,
                                            std::string_view loginName,
                                            std::string_view userName,
                                            std::uint16_t holepunchPort)

{
    PacketBuilder res(PacketId::UserStart, 256);
    auto& buf = res.GetBuffer();

    buf.Write(userId);
    buf.WriteString(loginName);
    buf.WriteString(userName);
    buf.Write<std::uint8_t>(1);  // unk00
    buf.Write(holepunchPort);
    res.Finish();

    return res;
}
