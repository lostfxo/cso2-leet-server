#include "platform.hpp"

#include <ifaddrs.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

std::pair<bool, std::string> FindIpOfInterface(std::string_view intf)
{
    ifaddrs* addresses = nullptr;
    getifaddrs(&addresses);

    for (; addresses != nullptr; addresses = addresses->ifa_next)
    {
        auto curAddr = addresses->ifa_addr;

        if (curAddr == nullptr || intf.compare(addresses->ifa_name) != 0 ||
            curAddr->sa_family != AF_INET)
        {
            continue;
        }

        char addressHost[NI_MAXHOST];
        getnameinfo(curAddr, sizeof(sockaddr_in), addressHost,
                    sizeof(addressHost), nullptr, 0, NI_NUMERICHOST);

        return { true, addressHost };
    }

    return { false, {} };
}
