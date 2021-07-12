#ifndef __IN_PACKETS_USERPROFILE_SETAVATAR_H_
#define __IN_PACKETS_USERPROFILE_SETAVATAR_H_

#include <cstdint>

class PacketView;

class InUserProfilePacketSetAvatar
{
public:
    InUserProfilePacketSetAvatar(PacketView& packet);

    std::uint16_t AvatarId;
};

#endif  // __IN_PACKETS_USERPROFILE_SETAVATAR_H_
