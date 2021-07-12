#ifndef __PACKETS_USERPROFILESHARED_H_
#define __PACKETS_USERPROFILESHARED_H_

#include <cstdint>

enum class UserProfilePacketType : std::uint8_t
{
    CampaignUpdate = 4,
    SetSignature = 5,
    SetTitle = 6,
    SetAvatar = 7
};

enum class UP_UpdateCampaignType : std::uint8_t
{
    Started = 0,
    Finished = 1
};

#endif  // __PACKETS_USERPROFILESHARED_H_
