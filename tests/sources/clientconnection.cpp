#include "clientconnection.hpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;

ClientConnection::ClientConnection(asio::io_context& ioContext,
                                   const tcp::endpoint& endpoint)
    : m_Socket(ioContext)
{
    co_spawn(
        this->m_Socket.get_executor(),
        [&, this] { return this->ConnectTo(endpoint); }, detached);
}

awaitable<void> ClientConnection::ConnectTo(const tcp::endpoint& endpoint)
{
    co_await this->m_Socket.async_connect(endpoint, use_awaitable);
}
