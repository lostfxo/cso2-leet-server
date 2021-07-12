#ifndef __HOLEPUNCHSERVER_H_
#define __HOLEPUNCHSERVER_H_

#include <cstdint>
#include <span>

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

namespace asio = boost::asio;
using boost::asio::awaitable;
using boost::asio::ip::udp;

class HolepunchInPacket;

class HolepunchServer
{
public:
    HolepunchServer(asio::io_context& io_context,
                    const udp::endpoint& endpoint);

    [[nodiscard]] std::uint16_t GetPort() const;

private:
    awaitable<void> AsyncOnReceive();
    awaitable<void> ParsePacket(std::span<uint8_t> buffer);

    awaitable<void> HandlePunchRequest(HolepunchInPacket& pkt);
    awaitable<void> HandleForwardRequest(HolepunchInPacket& pkt);

private:
    udp::socket m_Socket;
    udp::endpoint m_CurEndpoint;
};

#endif  // __HOLEPUNCHSERVER_H_
