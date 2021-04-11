#ifndef __IN_PACKETS_USERPROFILE_SETSIGNATURE_H_
#define __IN_PACKETS_USERPROFILE_SETSIGNATURE_H_

#include <string>

class PacketView;

class InUserProfilePacketSetSignature
{
public:
    InUserProfilePacketSetSignature(PacketView& packet);

    std::string Signature;
};

#endif  // __IN_PACKETS_USERPROFILE_SETSIGNATURE_H_
