#include "packets/out/version.hpp"

PacketBuilder OutVersionPacket::Version(std::string_view hash, bool isBadHash)

{
    PacketBuilder res(PacketId::Version, 40);
    DynamicBuffer& bufRef = res.GetBuffer();

    bufRef.Write<std::uint8_t>(isBadHash ? 1 : 0);
    bufRef.WriteString(hash);
    res.Finish();

    return res;
}
