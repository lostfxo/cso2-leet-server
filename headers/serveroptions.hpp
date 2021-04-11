#ifndef __SERVEROPTIONS_H_
#define __SERVEROPTIONS_H_

#include <cstdint>
#include <string_view>

struct ServerOptions
{
    std::string_view Hostname;
    std::uint16_t MasterPort;
    std::uint16_t UdpPort;
    bool ShouldLogPackets;
};

extern ServerOptions g_ServerOptions;

#endif  // __SERVEROPTIONS_H_
