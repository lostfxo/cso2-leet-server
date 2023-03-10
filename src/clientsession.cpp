#include "clientsession.hpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/write.hpp>

#include "activesessions.hpp"
#include "channel/channel.hpp"
#include "cso2/user.hpp"
#include "holepunch/shared.hpp"
#include "packetlogger.hpp"
#include "packets/builder.hpp"
#include "packets/view.hpp"
#include "room/room.hpp"
#include "serveroptions.hpp"
#include "services/userservice.hpp"
#include "util/log.hpp"
#include "util/randomstring.hpp"

#include "handlers.hpp"

using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::redirect_error;
using boost::asio::use_awaitable;
using namespace std::literals::string_view_literals;

constexpr const auto CLIENT_WELCOME_MESSAGE = "~SERVERCONNECTED\n\0"sv;

ClientSession::ClientSession(tcp::socket socket)
    : m_Socket(std::move(socket)), m_WriteTimer(socket.get_executor()),
      m_UniqueId(GenerateRandomString(16)), m_User(nullptr),
      m_CurChannel(nullptr), m_CurRoom(nullptr), m_LoggerNextInSequence(0),
      m_LoggerNextOutSequence(0), m_CachedRemoteIp(0), m_NextSequence(0),
      m_Initialized(false)
{
    this->m_WriteTimer.expires_at(std::chrono::steady_clock::time_point::max());
    this->m_CachedRemoteIp =
        this->m_Socket.remote_endpoint().address().to_v4().to_uint();
}

ClientSession::~ClientSession()
{
    if (this->m_CurChannel != nullptr)
    {
        this->m_CurChannel->RemoveSessionFromChannel(shared_from_this());
    }
}

void ClientSession::Initialize()
{
    if (this->m_Initialized == true)
    {
        Log::Warning(
            "[ClientSession::Initialize] tried to reinitialize session '{}'\n",
            this->GetUniqueId());
        return;
    }

    co_spawn(
        this->m_Socket.get_executor(), [this] { return this->AsyncReadLoop(); },
        detached);

    co_spawn(
        this->m_Socket.get_executor(),
        [this] { return this->AsyncWriteLoop(); }, detached);

    this->SendData(
        { CLIENT_WELCOME_MESSAGE.begin(), CLIENT_WELCOME_MESSAGE.end() });

    this->m_Initialized = true;
}

void ClientSession::Send(PacketBuilder&& pkt)
{
    pkt.SetSequence(this->GetNextSeq());

    auto data = pkt.GetBufferOwnership();

    if (g_ServerOptions.ShouldLogPackets)
    {
        PacketView packet(data);
        PacketLogger::OnOutPacket(this->GetUniqueId(),
                                  this->m_LoggerNextOutSequence++,
                                  std::uint32_t(packet.GetId()), data);
    }

    this->SendData(std::move(data));
}

void ClientSession::SendData(std::vector<std::uint8_t>&& data)
{
    this->m_QueuedWriteData.push_back(data);
    this->m_WriteTimer.cancel_one();
}

awaitable<std::uint64_t> ClientSession::ProcessData(
    const std::span<const std::uint8_t> data)
{
    try
    {
        PacketView packet(data);
        std::size_t fullPktLen = packet.GetLength() + BASE_PACKET_HEADER_LENGTH;

        if (g_ServerOptions.ShouldLogPackets)
        {
            PacketLogger::OnInPacket(
                this->GetUniqueId(), this->m_LoggerNextInSequence++,
                std::uint32_t(packet.GetId()), { data.data(), fullPktLen });
        }

        // the cases are ordered from most frequent to less frequent packet IDs
        // TODO: make sure the order is correct
        switch (packet.GetId())
        {
            case PacketId::Host:
                co_await OnHostPacketRequest(packet, shared_from_this());
                break;
            case PacketId::Room:
                OnRoomPacketRequest(packet, shared_from_this());
                break;
            case PacketId::Chat:
                OnChatPacketRequest(packet, shared_from_this());
                break;
            case PacketId::UserProfile:
                co_await OnUserProfilePacketRequest(packet, shared_from_this());
                break;
            case PacketId::RequestRoomList:
                OnRoomListPacketRequest(packet, shared_from_this());
                break;
            case PacketId::RequestChannels:
                OnChannelListPacketRequest(packet, shared_from_this());
                break;
            case PacketId::Favorite:
                co_await OnFavoritePacketRequest(packet, shared_from_this());
                break;
            case PacketId::Option:
                co_await OnOptionPacketRequest(packet, shared_from_this());
                break;
            case PacketId::Achievement:
                OnAchievementPacketRequest(packet, shared_from_this());
                break;
            case PacketId::Login:
                co_await OnLoginPacketRequest(packet, shared_from_this());
                break;
            case PacketId::Version:
                OnVersionPacketRequest(packet, shared_from_this());
                break;
            default:
                Log::Debug("[ClientSession::ProcessData] Received packet "
                           "with unhandled ID {} from "
                           "connection '{}'\n",
                           packet.GetId(), this->GetUniqueId());
        }

        co_return fullPktLen;
    }
    catch (const std::exception& e)
    {
        Log::Error("[ClientSession::ProcessData] caught '{}'\n", e.what());
        co_return 0;
    }
}

awaitable<void> ClientSession::AsyncReadLoop()
{
    std::uint8_t buffer[65536];

    for (;;)
    {
        size_t n = 0;

        try
        {
            n = co_await this->m_Socket.async_read_some(asio::buffer(buffer),
                                                        use_awaitable);
        }
        catch (const std::exception& e)
        {
            co_await this->Stop(e);
            co_return;
        }

        std::size_t curOffset = 0;

        try
        {
            while (n >= BASE_PACKET_HEADER_LENGTH)
            {
                auto bytesProcessed =
                    co_await this->ProcessData({ &buffer[curOffset], n });

                if (bytesProcessed == 0)
                {
                    Log::Warning("no bytes processed, bytesproc: {} n: {}\n",
                                 bytesProcessed, n);
                    break;
                }

                if (bytesProcessed > n)
                {
                    Log::Warning(
                        "too many bytes processed, bytesproc: {} n: {}\n",
                        bytesProcessed, n);
                    break;
                }

                curOffset += bytesProcessed;
                n -= bytesProcessed;
            }
        }
        catch (const std::exception& e)
        {
            Log::Warning("[ClientSession::AsyncReadLoop] skipping session '{}' "
                         "invalid data\n",
                         this->GetUniqueId());
            continue;
        }
    }
}

awaitable<void> ClientSession::AsyncWriteLoop()
{
    try
    {
        while (this->m_Socket.is_open())
        {
            if (this->m_QueuedWriteData.empty())
            {
                boost::system::error_code ec;
                co_await this->m_WriteTimer.async_wait(
                    redirect_error(use_awaitable, ec));
            }
            else
            {
                co_await asio::async_write(
                    this->m_Socket,
                    asio::buffer(this->m_QueuedWriteData.front()),
                    use_awaitable);
                this->m_QueuedWriteData.pop_front();
            }
        }
    }
    catch (const std::exception& e)
    {
        co_await this->Stop(e);
    }
}

awaitable<void> ClientSession::Stop(const std::exception& exception)
{
    this->m_WriteTimer.cancel();

    if (this->m_CurChannel != nullptr)
    {
        this->m_CurChannel->RemoveSessionFromChannel(shared_from_this());
    }

    if (this->HasUser() == true)
    {
        co_await g_UserService->Logout(this->GetUser()->GetId());
    }

    g_Sessions.RemoveSession(shared_from_this());

    Log::Warning("[ClientSession::Stop] session stopped with reason: {}\n",
                 exception.what());
}

bool ClientSession::UpdateHolepunch(std::uint32_t internalIp,
                                    std::uint32_t externalIp,
                                    std::uint16_t localPort,
                                    std::uint16_t externalPort,
                                    HolepunchPortId portId)
{
    this->m_InternalNetInfo.IpAddress = internalIp;
    this->m_ExternalNetInfo.IpAddress = externalIp;

    switch (portId)
    {
        case HolepunchPortId::Client:
            this->m_InternalNetInfo.ClientPort = localPort;
            this->m_ExternalNetInfo.ClientPort = externalPort;
            return true;
        case HolepunchPortId::Server:
            this->m_InternalNetInfo.ServerPort = localPort;
            this->m_ExternalNetInfo.ServerPort = externalPort;
            return true;
        case HolepunchPortId::SourceTV:
            this->m_InternalNetInfo.SourceTvPort = localPort;
            this->m_ExternalNetInfo.SourceTvPort = externalPort;
            return true;
        default:
            return false;
    }
}
