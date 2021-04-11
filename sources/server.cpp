#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include "holepunchserver.hpp"
#include "serverinstance.hpp"
#include "serveroptions.hpp"
#include "services/userservice.hpp"

void StartServer()
{
    asio::io_context io_context;

    tcp::endpoint endpoint(asio::ip::make_address_v4(g_ServerOptions.Hostname),
                           g_ServerOptions.MasterPort);
    udp::endpoint holepunchEndpoint(
        asio::ip::make_address_v4(g_ServerOptions.Hostname),
        g_ServerOptions.UdpPort);

    g_UserService =
        std::make_unique<UserService>("127.0.0.1", "30100", io_context);

    ServerInstance server(io_context, endpoint,
                          g_ServerOptions.ShouldLogPackets);
    HolepunchServer holepunchServer(io_context, holepunchEndpoint);

    g_ServerOptions.MasterPort = server.GetPort();
    g_ServerOptions.UdpPort = holepunchServer.GetPort();

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
