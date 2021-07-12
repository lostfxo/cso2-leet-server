#ifndef __TEMPLATE_PINGABLE_H_
#define __TEMPLATE_PINGABLE_H_

#include <boost/system/error_code.hpp>
#include <chrono>
#include <stdexcept>

#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/steady_timer.hpp>

#include <util/log.hpp>

namespace asio = boost::asio;
namespace chrono = std::chrono;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::redirect_error;
using boost::asio::use_awaitable;

class Pingable
{
public:
    Pingable(chrono::seconds pingDelay, asio::io_context& ctx)
        : m_PingTimer(ctx, chrono::steady_clock::now()), m_PingDelay(pingDelay),
          m_Health(HealthStatus::Unknown)
    {
        co_spawn(
            ctx, [this] { return this->TickTimer(); }, detached);
    }

    inline bool IsAlive() const noexcept
    {
        return this->m_Health == HealthStatus::Alive;
    }

protected:
    enum class HealthStatus
    {
        Unknown,
        Alive,
        Offline
    };

    virtual awaitable<bool> OnPing() = 0;
    virtual std::string_view GetServiceName() = 0;

private:
    awaitable<void> TickTimer()
    {
        while (true)
        {
            auto oldStatus = this->m_Health;

            try
            {
                this->m_Health = (co_await this->OnPing()) ?
                                     HealthStatus::Alive :
                                     HealthStatus::Offline;

                if (this->m_Health != oldStatus)
                {
                    Log::Warning("{} is now {}\n", this->GetServiceName(),
                                 this->m_Health == HealthStatus::Alive ?
                                     "online" :
                                     "offline");
                }
            }
            catch (const std::exception& e)
            {
                Log::Warning("{} TickTimer's threw with error '{}'\n",
                             this->GetServiceName(), e.what());
            }

            this->m_PingTimer.expires_after(this->m_PingDelay);

            boost::system::error_code ec;
            co_await this->m_PingTimer.async_wait(
                redirect_error(use_awaitable, ec));
        }
    }

    asio::steady_timer m_PingTimer;
    chrono::seconds m_PingDelay;
    HealthStatus m_Health;
};

#endif  // __TEMPLATE_PINGABLE_H_
