#ifndef __IN_PACKET_HOST_CHANGETEAM_H_
#define __IN_PACKET_HOST_CHANGETEAM_H_

#include "cso2/definitions.hpp"

class PacketView;

class InHostPacketChangeTeam
{
public:
    InHostPacketChangeTeam(PacketView& packet);

    std::uint8_t Type;
    std::uint16_t UserId;
    std::uint8_t unk00;
    std::uint8_t unk01;
    cso2::TeamNum NewTeam;
};

#endif  // __IN_PACKET_HOST_CHANGETEAM_H_
