#include "holepunchserver.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>

#include "activesessions.hpp"
#include "clientsession.hpp"
#include "holepunch/inpacket.hpp"
#include "holepunch/outpacket.hpp"
#include "util/log.hpp"

using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;

HolepunchServer::HolepunchServer(boost::asio::io_context& io_context,
                                 const udp::endpoint& endpoint)
    : m_Socket(io_context, endpoint)
{
    co_spawn(
        this->m_Socket.get_executor(),
        [this] { return this->AsyncOnReceive(); }, detached);

    Log::Info("[HolepunchServer::HolepunchServer] listening on {}:{}\n",
              endpoint.address().to_string(), endpoint.port());
}

std::uint16_t HolepunchServer::GetPort() const
{
    return this->m_Socket.local_endpoint().port();
}

awaitable<void> HolepunchServer::AsyncOnReceive()
{
    try
    {
        for (;;)
        {
            co_await this->m_Socket.async_receive_from(
                asio::buffer(this->m_CurBuffer), this->m_CurEndpoint,
                use_awaitable);

            co_await this->ParsePacket();
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("[HolepunchServer::onAsyncReceive] threw {}\n", e.what());
    }
}

awaitable<void> HolepunchServer::ParsePacket()
{
    try
    {
        HolepunchInPacket pkt(this->m_CurBuffer);

        if (pkt.IsHeartbeat() == true)
        {
            co_return;
        }

        auto session = g_Sessions.FindSessionByUserId(pkt.m_UserId);

        if (session == nullptr)
        {
            Log::Warning(
                "Failed to get user {}'s session from holepunch packet\n",
                pkt.m_UserId);
            co_return;
        }

        const auto externalIp = session->GetExternalAddress();

        if (externalIp != this->m_CurEndpoint.address().to_v4().to_uint())
        {
            Log::Warning("IP address from session is different from holepunch "
                         "'{}' packet origin, is someone spoofing packets?\n",
                         this->m_CurEndpoint.address().to_string());
            co_return;
        }

        auto wasUpdated =
            session->UpdateHolepunch(pkt.m_IpAddress, externalIp, pkt.m_PortNum,
                                     this->m_CurEndpoint.port(), pkt.m_PortId);

        if (wasUpdated == false)
        {
            Log::Warning("'{}' used an unknown holepunch port\n",
                         this->m_CurEndpoint.address().to_string());
            co_return;
        }

        auto outPkt = HolepunchOutPacket(pkt.m_PortId);
        auto bufView = outPkt.GetDataView();

        co_await this->m_Socket.async_send_to(
            asio::const_buffer(bufView.data(), bufView.size_bytes()),
            this->m_CurEndpoint, use_awaitable);
    }
    catch (const std::exception& e)
    {
        Log::Warning("[HolepunchServer::ParsePacket] threw {}\n", e.what());
    }
}
