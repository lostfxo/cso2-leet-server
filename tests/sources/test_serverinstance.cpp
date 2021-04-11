#include <catch2/catch_test_macros.hpp>

#include <boost/asio/io_context.hpp>

#include "holepunchserver.hpp"
#include "serverinstance.hpp"
#include "services/userservice.hpp"

#include "clientconnection.hpp"
#include "wait_util.hpp"

std::uint16_t g_HolepunchPort = 0;

TEST_CASE("ServerInstance can be connected to", "[serverinstance]")
{
    std::unique_ptr<std::thread> serverThread;
    std::unique_ptr<asio::io_context> serverIoContext;

    std::unique_ptr<std::thread> clientThread;
    std::unique_ptr<asio::io_context> clientIoContext;

    std::uint16_t serverMasterPort = 0;
    std::uint16_t serverUdpPort = 0;

    SECTION("Starting the server")
    {
        serverThread = std::make_unique<std::thread>([&]() {
            serverIoContext = std::make_unique<asio::io_context>();

            tcp::endpoint endpoint(tcp::v4(), 0);
            udp::endpoint holepunchEndpoint(udp::v4(), 0);

            g_UserService = std::make_unique<UserService>("127.0.0.1", "30100",
                                                          *serverIoContext);

            ServerInstance server(*serverIoContext, endpoint, false);
            HolepunchServer holepunchServer(*serverIoContext,
                                            holepunchEndpoint);

            serverMasterPort = server.GetPort();
            serverUdpPort = holepunchServer.GetPort();
            g_HolepunchPort = serverUdpPort;

            serverIoContext->run();
        });
        REQUIRE(serverThread != nullptr);
    }

    SECTION("Waiting for the server to start")
    {
        WaitByTask([&]() { return serverMasterPort != 0; });

        REQUIRE(serverMasterPort != 0);
        REQUIRE(serverUdpPort != 0);
    }

    SECTION("Connecting to the master server")
    {
        ClientConnection* connPtr = nullptr;

        clientThread = std::make_unique<std::thread>([&]() {
            clientIoContext = std::make_unique<asio::io_context>();
            ClientConnection conn(*clientIoContext,
                                  tcp::endpoint(tcp::v4(), serverMasterPort));
            clientIoContext->run();
        });

        WaitByTask(
            [&]() { return connPtr != nullptr && connPtr->IsOpen() == true; });
        REQUIRE(connPtr->IsOpen() == true);
    }

    SECTION("Shutting down the connection")
    {
        clientIoContext->stop();
        REQUIRE(clientIoContext->stopped() == true);
        clientThread->join();
    }

    SECTION("Shutdowning down the server")
    {
        serverIoContext->stop();
        REQUIRE(serverIoContext->stopped() == true);
        serverThread->join();
    }
}
