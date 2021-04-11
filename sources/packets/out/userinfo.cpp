#include "packets/out/userinfo.hpp"

#include "cso2/user.hpp"
#include "packets/userinfoshared.hpp"

PacketBuilder OutUserInfoPacket::FullUpdate(cso2::UserPtr user)
{
    PacketBuilder res(PacketId::UserInfo, 1024);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint32_t>(user->GetId());
    WriteUserInfoUpdate(buf, 0xFFFFFFFF, user);
    res.Finish();

    return res;
}

PacketBuilder OutUserInfoPacket::UpdateAvatar(cso2::UserPtr user)
{
    PacketBuilder res(PacketId::UserInfo, 64);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint32_t>(user->GetId());
    WriteUserInfoUpdate(buf, 0x800000, user);
    res.Finish();

    return res;
}

PacketBuilder OutUserInfoPacket::UpdateSignature(cso2::UserPtr user)
{
    PacketBuilder res(PacketId::UserInfo, 256);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint32_t>(user->GetId());
    WriteUserInfoUpdate(buf, 0x40000, user);
    res.Finish();

    return res;
}

PacketBuilder OutUserInfoPacket::UpdateTitle(cso2::UserPtr user)
{
    PacketBuilder res(PacketId::UserInfo, 64);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint32_t>(user->GetId());
    WriteUserInfoUpdate(buf, 0x8000, user);
    res.Finish();

    return res;
}

PacketBuilder OutUserInfoPacket::UpdateCampaignFlags(cso2::UserPtr user)
{
    PacketBuilder res(PacketId::UserInfo, 64);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint32_t>(user->GetId());
    WriteUserInfoUpdate(buf, 0x1000, user);
    res.Finish();

    return res;
}

PacketBuilder OutUserInfoPacket::UpdateKillStats(cso2::UserPtr user)
{
    PacketBuilder res(PacketId::UserInfo, 64);
    auto& buf = res.GetBuffer();

    buf.Write<std::uint32_t>(user->GetId());
    WriteUserInfoUpdate(buf, 0x40, user);
    res.Finish();

    return res;
}
