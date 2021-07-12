#ifndef __IN_PACKET_HOST_ONPLAYERSCORED_H_
#define __IN_PACKET_HOST_ONPLAYERSCORED_H_

#include "cso2/definitions.hpp"

class PacketView;

class InHostPacketOnPlayerScored
{
public:
    InHostPacketOnPlayerScored(PacketView& packet);

    // members are memory aligned
    std::uint32_t ScorerUserId;
    std::uint32_t unk04;
    std::uint16_t TotalFrags;
    std::uint8_t NewFragsCount;
    bool FakeClient;
    cso2::TeamNum ScorerTeam;
    std::uint8_t unk03;
};

#endif  // __IN_PACKET_HOST_ONPLAYERSCORED_H_
