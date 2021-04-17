#include <cstdlib>
#include <iostream>
#include <string_view>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include "cmdparser.hpp"
#include "globals.hpp"
#include "holepunchserver.hpp"
#include "serverinstance.hpp"
#include "util/log.hpp"

#include "services/userservice.hpp"

#define LEET_VERSION "0.0.1"

constexpr std::string_view DEFAULT_IP_ADDRESS = "0.0.0.0";
constexpr std::string_view DEFAULT_USERSVC_ADDRESS = "127.0.0.1";
constexpr const std::uint16_t DEFAULT_MASTER_PORT = 30001;
constexpr const std::uint16_t DEFAULT_UDP_PORT = 30002;
constexpr const std::uint16_t DEFAULT_USERSVC_PORT = 30100;

std::uint16_t g_HolepunchPort = 0;

struct ServerOptions
{
    // members are memory aligned
    LogVerbosity Verbosity;

    std::string_view Hostname;
    std::string_view UserSvcHost;

    std::uint16_t MasterPort;
    std::uint16_t UdpPort;
    std::uint16_t UserSvcPort;

    bool ShouldLogPackets;
};

ServerOptions GetCmdOptions(const CmdParser& cmd)
{
    auto logVerb = LogVerbosity::Info;

    if (cmd.HasOption("-l") == true)
    {
        logVerb = StringToLogVerbosity(cmd.GetOption("-l"));
    }
    else if (cmd.HasOption("--logging") == true)
    {
        logVerb = StringToLogVerbosity(cmd.GetOption("--logging"));
    }

    std::string_view ipAddress = DEFAULT_IP_ADDRESS;

    if (cmd.HasOption("-i") == true)
    {
        ipAddress = cmd.GetOption("-i");
    }
    else if (cmd.HasOption("--ip-address") == true)
    {
        ipAddress = cmd.GetOption("--ip-address");
    }

    std::uint16_t masterPort = DEFAULT_MASTER_PORT;

    if (cmd.HasOption("-p") == true)
    {
        masterPort = cmd.GetUintOption("-p");
    }
    else if (cmd.HasOption("--port-master") == true)
    {
        masterPort = cmd.GetUintOption("--port-master");
    }

    if (masterPort == 0)
    {
        throw std::invalid_argument("The master port number is invalid.");
    }

    std::uint16_t udpPort = DEFAULT_UDP_PORT;

    if (cmd.HasOption("-P") == true)
    {
        udpPort = cmd.GetUintOption("-p");
    }
    else if (cmd.HasOption("--port-udp") == true)
    {
        udpPort = cmd.GetUintOption("--port-udp");
    }

    if (udpPort == 0)
    {
        throw std::invalid_argument(
            "The UDP (holepunch) port number is invalid.");
    }

    std::string_view userSvcHost;
    auto userSvcHostPtr = std::getenv("USERSERVICE_HOST");

    if (userSvcHostPtr != nullptr)
    {
        userSvcHost = userSvcHostPtr;
    }
    else
    {
        userSvcHost = DEFAULT_USERSVC_ADDRESS;
    }

    std::uint16_t userSvcPort;
    auto userSvcPortPtr = std::getenv("USERSERVICE_PORT");

    if (userSvcPortPtr != nullptr)
    {
        userSvcPort = std::stoul(userSvcPortPtr);
    }
    else
    {
        userSvcPort = DEFAULT_USERSVC_PORT;
    }

    bool shouldLogPackets =
        cmd.HasOption("-L") || cmd.HasOption("--log-packets");

    return { .Verbosity = logVerb,
             .Hostname = ipAddress,
             .UserSvcHost = userSvcHost,
             .MasterPort = masterPort,
             .UdpPort = udpPort,
             .UserSvcPort = userSvcPort,
             .ShouldLogPackets = shouldLogPackets };
}

void PrintHeader()
{
    std::cout << "leet-server " LEET_VERSION
                 " - a master server for Counter-Strike Online 2\n";
}

void PrintHelp()
{
    std::cout
        << "Options available:\n"
           "\t-l, --logging - Sets the log output verbosity, options: [debug, "
           "info, warning, error]\n"
           "\t-i, --ip-address [...] The IP address to be used by the server\n"
           "\t-p, --port-master [...] - The TCP port to be used by the "
           "server's client connections\n"
           "\t-P, --port-holepunch [...] - The UDP port to be used by "
           "the server's holepuncher\n"
           "\t--usersvc-host [...] The hostname of an user service\n"
           "\t--usersvc-port [...] The port number of an user service\n"
           "\t-v, --version - Print the program's version\n"
           "\t-h, --help - Print this help message\n";
}

void PrintVersion()
{
    std::cout << LEET_VERSION "\n";
}

int main(int argc, char* argv[])
{
    CmdParser cmd(argc, argv);

    if (cmd.HasOption("-v") == true || cmd.HasOption("--version"))
    {
        PrintVersion();
        return 0;
    }

    PrintHeader();

    if (cmd.HasOption("-h") == true || cmd.HasOption("--help"))
    {
        PrintHelp();
        return 0;
    }

    try
    {
        auto options = GetCmdOptions(cmd);

        Log::SetVerbosity(options.Verbosity);

        asio::io_context io_context;

        auto hostIp = asio::ip::make_address_v4(options.Hostname);

        tcp::endpoint endpoint(hostIp, options.MasterPort);
        udp::endpoint holepunchEndpoint(hostIp, options.UdpPort);

        g_HolepunchPort = options.UdpPort;
        g_UserService = std::make_unique<UserService>(
            options.UserSvcHost, std::to_string(options.UserSvcPort),
            io_context);

        ServerInstance server(io_context, endpoint, options.ShouldLogPackets);
        HolepunchServer holepunchServer(io_context, holepunchEndpoint);

        asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { io_context.stop(); });

        std::array<std::thread, 4> threads;

        for (size_t i = 0; i < threads.max_size(); i++)
        {
            threads[i] = std::thread([&]() { io_context.run(); });
        }

        for (size_t i = 0; i < threads.max_size(); i++)
        {
            threads[i].join();
        }
    }
    catch (std::exception& e)
    {
        Log::Error("The server threw an error: {}\n", e.what());
    }

    Log::Info("Stopped master server\n");

    return 0;
}
