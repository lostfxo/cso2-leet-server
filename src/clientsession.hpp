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
    ClientSession(tcp::socket socket);
    ~ClientSession();

    void Initialize();

    void Send(PacketBuilder&& pkt);
    void SendData(std::vector<std::uint8_t>&& data);

    inline std::string_view GetUniqueId() const { return this->m_UniqueId; }

    inline cso2::UserPtr GetUser() const { return this->m_User; }

    inline bool HasUser() const { return this->m_User != nullptr; }

    inline ChannelPtr GetCurChannel() const { return this->m_CurChannel; }

    inline bool IsInChannel() const { return this->m_CurChannel != nullptr; }

    inline std::shared_ptr<Room> GetCurRoom() const { return this->m_CurRoom; }

    inline bool IsInRoom() const { return this->m_CurRoom != nullptr; }

    inline std::uint32_t GetInternalAddress() const
    {
        return this->m_InternalNetInfo.IpAddress;
    }

    inline std::uint16_t GetInternalClientPort() const
    {
        return this->m_InternalNetInfo.ClientPort;
    }

    inline std::uint16_t GetInternalServerPort() const
    {
        return this->m_InternalNetInfo.ServerPort;
    }

    inline std::uint16_t GetInternalTvPort() const
    {
        return this->m_InternalNetInfo.SourceTvPort;
    }

    inline std::uint32_t GetExternalAddress() const
    {
        return this->m_CachedRemoteIp;
    }

    inline std::uint16_t GetExternalClientPort() const
    {
        return this->m_ExternalNetInfo.ClientPort;
    }

    inline std::uint16_t GetExternalServerPort() const
    {
        return this->m_ExternalNetInfo.ServerPort;
    }

    inline std::uint16_t GetExternalTvPort() const
    {
        return this->m_ExternalNetInfo.SourceTvPort;
    }

    inline void SetUser(cso2::UserPtr user) { this->m_User = user; }

    inline void SetCurChannel(ChannelPtr channel)
    {
        this->m_CurChannel = channel;
    }

    inline void SetCurRoom(std::shared_ptr<Room> room)
    {
        this->m_CurRoom = room;
    }

    bool UpdateHolepunch(std::uint32_t internalIp, std::uint32_t externalIp,
                         std::uint16_t localPort, std::uint16_t externalPort,
                         HolepunchPortId portId);

protected:
    awaitable<std::uint64_t> ProcessData(
        const std::span<const std::uint8_t> data);

private:
    awaitable<void> AsyncReadLoop();
    awaitable<void> AsyncWriteLoop();

    awaitable<void> Stop(const std::exception& exception);

    inline std::uint8_t GetNextSeq()
    {
        if (this->m_NextSequence == 255)
        {
            this->m_NextSequence = 0;
            return this->m_NextSequence;
        }

        return this->m_NextSequence++;
    }

private:
    tcp::socket m_Socket;

    std::deque<std::vector<std::uint8_t>> m_QueuedWriteData;
    asio::steady_timer m_WriteTimer;

    std::string m_UniqueId;
    cso2::UserPtr m_User;
    ChannelPtr m_CurChannel;
    std::shared_ptr<Room> m_CurRoom;

    std::uint32_t m_LoggerNextInSequence;
    std::uint32_t m_LoggerNextOutSequence;

    std::uint32_t m_CachedRemoteIp;
    SessionNetInfo m_InternalNetInfo;
    SessionNetInfo m_ExternalNetInfo;

    std::uint8_t m_NextSequence;

    bool m_Initialized;
};

#endif  // __CLIENTSESSION_H_
