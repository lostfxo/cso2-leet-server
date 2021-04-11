#ifndef __IN_PACKET_HOST_ONROUNDEND_H_
#define __IN_PACKET_HOST_ONROUNDEND_H_

#include <cstdint>

#include "cso2/definitions.hpp"

class PacketView;

class InHostPacketOnRoundEnd
{
public:
    InHostPacketOnRoundEnd(PacketView& packet);

    cso2::TeamNum WinnerTeam;
    std::uint8_t TotalTerWins;
    std::uint8_t TotalCtWins;
    std::uint8_t unk;
};

#endif  // __IN_PACKET_HOST_ONROUNDEND_H_
