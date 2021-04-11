#ifndef __CLIENTCONNECTION_H_
#define __CLIENTCONNECTION_H_

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace asio = boost::asio;
using asio::awaitable;
using asio::ip::tcp;

class ClientConnection
{
public:
    ClientConnection(asio::io_context& ioContext,
                     const tcp::endpoint& endpoint);

    [[nodiscard]] inline bool IsOpen() const
    {
        return this->m_Socket.is_open();
    }

protected:
    awaitable<void> ConnectTo(const tcp::endpoint& endpoint);

private:
    tcp::socket m_Socket;
};

#endif  // __CLIENTCONNECTION_H_
