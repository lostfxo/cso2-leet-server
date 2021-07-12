#ifndef __IN_PACKETS_USERPROFILE_SETTITLE_H_
#define __IN_PACKETS_USERPROFILE_SETTITLE_H_

#include <cstdint>

class PacketView;

class InUserProfilePacketSetTitle
{
public:
    InUserProfilePacketSetTitle(PacketView& packet);

    std::uint16_t TitleId;
};

#endif  // __IN_PACKETS_USERPROFILE_SETTITLE_H_
