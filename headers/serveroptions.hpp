#ifndef __SERVEROPTIONS_H_
#define __SERVEROPTIONS_H_

#include <cstdint>
#include <string_view>

#include "util/log.hpp"

struct ServerOptions
{
    // members are memory aligned
    LogVerbosity Verbosity;

    std::string Hostname;
    std::string PublicHostname;
    std::string_view UserSvcHost;

    std::uint16_t MasterPort;
    std::uint16_t UdpPort;
    std::uint16_t PublicUdpPort;
    std::uint16_t UserSvcPort;

    bool ShouldLogPackets;
};

extern ServerOptions g_ServerOptions;

#endif  // __SERVEROPTIONS_H_
