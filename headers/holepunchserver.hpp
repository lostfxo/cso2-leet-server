#ifndef __HOLEPUNCHSERVER_H_
#define __HOLEPUNCHSERVER_H_

#include <array>
#include <cstdint>

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

namespace asio = boost::asio;
using boost::asio::awaitable;
using boost::asio::ip::udp;

class HolepunchServer
{
public:
    HolepunchServer(asio::io_context& io_context,
                    const udp::endpoint& endpoint);

    [[nodiscard]] std::uint16_t GetPort() const;

private:
    awaitable<void> AsyncOnReceive();
    awaitable<void> ParsePacket();

private:
    udp::socket m_Socket;
    udp::endpoint m_CurEndpoint;
    std::array<std::uint8_t, 16> m_CurBuffer;
};

#endif  // __HOLEPUNCHSERVER_H_
