#include "platform.hpp"

#include <iphlpapi.h>
#include <winsock2.h>

#include <stdexcept>

std::pair<bool, std::string> FindIpOfInterface(std::string_view intf)
{
    PIP_ADAPTER_ADDRESSES addresses = nullptr;
    ULONG outBufLen = 0;
    DWORD getRes = 0;

    int tries = 0;

    do
    {
        addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(malloc(outBufLen));

        if (addresses == nullptr)
        {
            printf(
                "Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            exit(1);
        }

        getRes = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr,
                                      addresses, &outBufLen);

        if (getRes == ERROR_BUFFER_OVERFLOW)
        {
            free(addresses);
            addresses = nullptr;
        }
        else
        {
            break;
        }

        tries++;

    } while (getRes == ERROR_BUFFER_OVERFLOW && tries < 3);

    if (getRes != NO_ERROR)
    {
        throw std::runtime_error("Failed to get interfaces");
    }

    bool ipFound = false;
    std::string targetIp;

    for (auto curAddr = addresses; curAddr != nullptr; curAddr = curAddr->Next)
    {
        if (intf.compare(curAddr->AdapterName) == 0)
        {
            auto ucAddr = curAddr->FirstUnicastAddress;

            char addressHost[NI_MAXHOST];
            getnameinfo(ucAddr->Address.lpSockaddr,
                        ucAddr->Address.iSockaddrLength, addressHost,
                        sizeof(addressHost), nullptr, 0, NI_NUMERICHOST);

            targetIp = addressHost;
            ipFound = true;
            break;
        }
    }

    free(addresses);

    return { ipFound, std::move(targetIp) };
}
