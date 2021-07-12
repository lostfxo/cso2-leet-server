#ifndef __SERVERINSTANCE_H_
#define __SERVERINSTANCE_H_

#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class PacketLogger;

namespace asio = boost::asio;
using boost::asio::ip::tcp;

class ServerInstance
{
public:
    ServerInstance(asio::io_context& io_context, const tcp::endpoint& endpoint,
                   bool logPackets = false);

    [[nodiscard]] std::uint16_t GetPort() const;

private:
    void asyncOnConnection();

private:
    tcp::acceptor m_Acceptor;

    std::shared_ptr<PacketLogger> m_PacketLogger;
};

#endif  // __SERVERINSTANCE_H_
