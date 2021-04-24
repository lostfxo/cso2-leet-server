#ifndef __CLIENTSESSION_H_
#define __CLIENTSESSION_H_

#include <deque>
#include <span>
#include <string_view>
#include <vector>

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>

#include "clientsession_shared.hpp"

#include "channel/shared.hpp"
#include "cso2/shared.hpp"
#include "holepunch/shared.hpp"

namespace asio = boost::asio;
using boost::asio::awaitable;
using boost::asio::ip::tcp;

class PacketLogger;
class Room;

class PacketBuilder;

struct SessionNetInfo
{
    SessionNetInfo()
        : IpAddress(0), ClientPort(0), ServerPort(0), SourceTvPort(0)
    {
    }

    std::uint32_t IpAddress;
    std::uint16_t ClientPort;
    std::uint16_t ServerPort;
    std::uint16_t SourceTvPort;
};

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(tcp::socket socket,
                  std::shared_ptr<PacketLogger> logger = nullptr);
    ~ClientSession();

    void Initialize();

    void Send(PacketBuilder&& pkt);
    void SendData(std::vector<std::uint8_t>&& data);

    [[nodiscard]] inline std::string_view GetUniqueId() const noexcept
    {
        return this->m_UniqueId;
    }

    [[nodiscard]] inline cso2::UserPtr GetUser() const noexcept
    {
        return this->m_User;
    }

    [[nodiscard]] inline bool HasUser() const noexcept
    {
        return this->m_User != nullptr;
    }

    [[nodiscard]] inline ChannelPtr GetCurChannel() const noexcept
    {
        return this->m_CurChannel;
    }

    [[nodiscard]] inline bool IsInChannel() const noexcept
    {
        return this->m_CurChannel != nullptr;
    }

    [[nodiscard]] inline std::shared_ptr<Room> GetCurRoom() const noexcept
    {
        return this->m_CurRoom;
    }

    [[nodiscard]] inline bool IsInRoom() const noexcept
    {
        return this->m_CurRoom != nullptr;
    }

    [[nodiscard]] inline std::uint32_t GetInternalAddress() const noexcept
    {
        return this->m_InternalNetInfo.IpAddress;
    }

    [[nodiscard]] inline std::uint16_t GetInternalClientPort() const noexcept
    {
        return this->m_InternalNetInfo.ClientPort;
    }

    [[nodiscard]] inline std::uint16_t GetInternalServerPort() const noexcept
    {
        return this->m_InternalNetInfo.ServerPort;
    }

    [[nodiscard]] inline std::uint16_t GetInternalTvPort() const noexcept
    {
        return this->m_InternalNetInfo.SourceTvPort;
    }

    [[nodiscard]] inline std::uint32_t GetExternalAddress() const noexcept
    {
        return this->m_CachedRemoteIp;
    }

    [[nodiscard]] inline std::uint16_t GetExternalClientPort() const noexcept
    {
        return this->m_ExternalNetInfo.ClientPort;
    }

    [[nodiscard]] inline std::uint16_t GetExternalServerPort() const noexcept
    {
        return this->m_ExternalNetInfo.ServerPort;
    }

    [[nodiscard]] inline std::uint16_t GetExternalTvPort() const noexcept
    {
        return this->m_ExternalNetInfo.SourceTvPort;
    }

    inline void SetUser(cso2::UserPtr user) noexcept { this->m_User = user; }

    inline void SetCurChannel(ChannelPtr channel) noexcept
    {
        this->m_CurChannel = channel;
    }

    inline void SetCurRoom(std::shared_ptr<Room> room) noexcept
    {
        this->m_CurRoom = room;
    }

    bool UpdateHolepunch(std::uint32_t internalIp, std::uint32_t externalIp,
                         std::uint16_t localPort, std::uint16_t externalPort,
                         HolepunchPortId portId) noexcept;

protected:
    awaitable<std::uint64_t> ProcessData(
        const std::span<const std::uint8_t> data);

private:
    awaitable<void> AsyncReadLoop();
    awaitable<void> AsyncWriteLoop();

    void Stop(const std::exception& exception) noexcept;

    [[nodiscard]] inline std::uint8_t GetNextSeq() noexcept
    {
        if (this->m_NextSequence == 255)
        {
            this->m_NextSequence = 0;
            return this->m_NextSequence;
        }

        return this->m_NextSequence++;
    }

    [[nodiscard]] inline std::uint32_t GetLoggerNextInSeq() noexcept
    {
        return this->m_LoggerNextInSequence++;
    }

    [[nodiscard]] inline std::uint32_t GetLoggerNextOutSeq() noexcept
    {
        return this->m_LoggerNextOutSequence++;
    }

    void LogPacketData(const std::span<const std::uint8_t> data);

private:
    tcp::socket m_Socket;

    std::deque<std::vector<std::uint8_t>> m_QueuedWriteData;
    asio::steady_timer m_WriteTimer;

    std::string m_UniqueId;
    cso2::UserPtr m_User;
    ChannelPtr m_CurChannel;
    std::shared_ptr<Room> m_CurRoom;

    std::shared_ptr<PacketLogger> m_Logger;
    std::uint32_t m_LoggerNextInSequence;
    std::uint32_t m_LoggerNextOutSequence;

    std::uint32_t m_CachedRemoteIp;
    SessionNetInfo m_InternalNetInfo;
    SessionNetInfo m_ExternalNetInfo;

    std::uint8_t m_NextSequence;

    bool m_Initialized;
};

#endif  // __CLIENTSESSION_H_
