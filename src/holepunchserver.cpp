#include "holepunchserver.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>

#include <array>
#include <stdexcept>

#include "activesessions.hpp"
#include "clientsession.hpp"
#include "room/room.hpp"
#include "util/log.hpp"

#include "holepunch/inpacket.hpp"
#include "holepunch/inpacket/forward.hpp"
#include "holepunch/inpacket/punch.hpp"
#include "holepunch/outpacket.hpp"

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
        std::array<std::uint8_t, 4096> pktBuffer;

        for (;;)
        {
            size_t bufSize = co_await this->m_Socket.async_receive_from(
                asio::buffer(pktBuffer), this->m_CurEndpoint, use_awaitable);

            co_await this->ParsePacket({ pktBuffer.data(), bufSize });
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("[HolepunchServer::onAsyncReceive] threw {}\n", e.what());
    }
}

awaitable<void> HolepunchServer::ParsePacket(std::span<uint8_t> buffer)
{
    try
    {
        HolepunchInPacket pkt(buffer);

        switch (pkt.m_Type)
        {
            case HolepunchPacketType::Punch:
                co_await this->HandlePunchRequest(pkt);
                break;
            case HolepunchPacketType::HeartbeatForClient:
            case HolepunchPacketType::HeartbeatForServer:
            case HolepunchPacketType::HeartbeatForSourceTV:
                // keep alive for the user's NAT, do nothing
                break;
            case HolepunchPacketType::ForwardToServer:
                co_await this->HandleForwardRequest(pkt);
                break;
            default:
                Log::Debug("[HolepunchServer::ParsePacket] Unknown type {}\n",
                           pkt.m_Type);
                break;
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("[HolepunchServer::ParsePacket] threw {}\n", e.what());
    }
}

awaitable<void> HolepunchServer::HandlePunchRequest(HolepunchInPacket& pkt)
{
    HolepunchInPunchPacket punchPkt(pkt);

    auto session = g_Sessions.FindSessionByUserId(pkt.m_UserId);

    if (session == nullptr)
    {
        Log::Warning("Failed to get user {}'s session from holepunch packet\n",
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

    auto wasUpdated = session->UpdateHolepunch(
        punchPkt.m_IpAddress, externalIp, punchPkt.m_PortNum,
        this->m_CurEndpoint.port(), punchPkt.m_PortId);

    if (wasUpdated == false)
    {
        Log::Warning("'{}' used an unknown holepunch port\n",
                     this->m_CurEndpoint.address().to_string());
        co_return;
    }

    auto outPkt = HolepunchOutPacket(punchPkt.m_PortId);
    auto bufView = outPkt.GetDataView();

    co_await this->m_Socket.async_send_to(
        asio::const_buffer(bufView.data(), bufView.size_bytes()),
        this->m_CurEndpoint, use_awaitable);
}

awaitable<void> HolepunchServer::HandleForwardRequest(HolepunchInPacket& pkt)
{
    HolepunchInFwdPacket fwdPkt(pkt);

    auto session = g_Sessions.FindSessionByUserId(pkt.m_UserId);

    if (session == nullptr)
    {
        Log::Warning(
            "Failed to get user {}'s session from relay forward packet\n",
            pkt.m_UserId);
        co_return;
    }

    auto targetSession = g_Sessions.FindSessionByUserId(fwdPkt.m_TargetUserId);

    if (targetSession == nullptr)
    {
        Log::Warning("Failed to get target user {}'s session to send relay "
                     "forward packet\n",
                     fwdPkt.m_TargetUserId);
        co_return;
    }

    auto targetAddress =
        asio::ip::make_address_v4(targetSession->GetExternalAddress());
    std::uint16_t targetPort = 0;

    switch (fwdPkt.m_TargetPortId)
    {
        case HolepunchPortId::Client:
            targetPort = targetSession->GetExternalClientPort();
            break;
        case HolepunchPortId::Server:
            targetPort = targetSession->GetExternalServerPort();
            break;
        case HolepunchPortId::SourceTV:
            targetPort = targetSession->GetExternalTvPort();
            break;
        default:
            Log::Debug("[HolepunchServer::HandleForwardRequest] Unknown port "
                       "ID {} used\n",
                       fwdPkt.m_TargetPortId);
            co_return;
    }

    udp::endpoint targetEndpoint(targetAddress, targetPort);

    co_await this->m_Socket.async_send_to(
        asio::const_buffer(fwdPkt.m_PacketDataView.data(),
                           fwdPkt.m_PacketDataView.size_bytes()),
        targetEndpoint, use_awaitable);
}
