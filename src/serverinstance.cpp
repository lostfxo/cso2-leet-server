#include "serverinstance.hpp"

#include "activesessions.hpp"
#include "clientsession.hpp"
#include "util/log.hpp"

ServerInstance::ServerInstance(boost::asio::io_context& io_context,
                               const tcp::endpoint& endpoint)
    : m_Acceptor(io_context, endpoint)
{
    this->asyncOnConnection();

    Log::Info(
        "[ServerInstance::ServerInstance] master server listening on {}:{}\n",
        endpoint.address().to_string(), endpoint.port());
}

std::uint16_t ServerInstance::GetPort() const
{
    return this->m_Acceptor.local_endpoint().port();
}

void ServerInstance::asyncOnConnection()
{
    this->m_Acceptor.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                auto newSession =
                    std::make_shared<ClientSession>(std::move(socket));
                newSession->Initialize();
                g_Sessions.AddSession(std::move(newSession));
            }

            asyncOnConnection();
        });
}
