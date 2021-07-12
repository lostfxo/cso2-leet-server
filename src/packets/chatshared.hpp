#ifndef __PACKETS_CHATSHARED_H_
#define __PACKETS_CHATSHARED_H_

#include <cstdint>

enum class ChatPacketType : std::uint8_t
{
    DirectMessage = 0,  // whisper
    Channel = 1,
    Room = 2,
    IngameGlobal = 3,
    IngameTeam = 4,
    Clan = 5,
    Party = 7,
    IngameParty = 9,

    // system messages
    Congratulate = 11,  // when someone get the rare item from box opening
    SystemImportant = 20,
    DialogBox = 21,  // note: has not 'Yes' and 'No' select option
    System = 22,
    DialogBoxExit = 60  // when client clicked 'OK', the game will exit (text
                        // start with '#CSO2_POPUP_')
};

#endif  // __PACKETS_CHATSHARED_H_
