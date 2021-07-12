#include "services/userservice.hpp"

#include <fmt/format.h>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/value_to.hpp>
#include <stdexcept>

#include "boost/json/array.hpp"
#include "cso2/buymenu.hpp"
#include "cso2/cosmetics.hpp"
#include "cso2/definitions.hpp"
#include "cso2/inventory.hpp"
#include "cso2/loadout.hpp"
#include "cso2/user.hpp"
#include "generic/pingable.hpp"
#include "util/log.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using asio::use_awaitable;

constexpr const auto DEFAULT_HTTP_UA = "cso2-leet-server";
constexpr const std::uint32_t DEFAULT_HTTP_VERSION = 11;

std::unique_ptr<UserService> g_UserService;

inline auto BuildGetRequest(std::string target)
{
    http::request<http::empty_body> req{ http::verb::get, target,
                                         DEFAULT_HTTP_VERSION };

    req.set(http::field::user_agent, DEFAULT_HTTP_UA);

    return req;
}

inline auto BuildPostJsonRequest(std::string target, const json::value& data)
{
    http::request<http::string_body> req{ http::verb::post, target,
                                          DEFAULT_HTTP_VERSION };

    req.set(http::field::user_agent, DEFAULT_HTTP_UA);
    req.set(http::field::content_type, "application/json");
    req.body() = json::serialize(data);
    req.prepare_payload();

    return req;
}

inline auto BuildPutJsonRequest(std::string target, const json::value& data)
{
    http::request<http::string_body> req{ http::verb::put, target,
                                          DEFAULT_HTTP_VERSION };

    req.set(http::field::user_agent, DEFAULT_HTTP_UA);
    req.set(http::field::content_type, "application/json");
    req.body() = json::serialize(data);
    req.prepare_payload();

    return req;
}

inline std::string_view GetCosmeticSlotKeyString(cso2::CosmeticSlot slot)
{
    switch (slot)
    {
        case cso2::CosmeticSlot::CtItem:
            return "ct_item"sv;
        case cso2::CosmeticSlot::TerItem:
            return "ter_item"sv;
        case cso2::CosmeticSlot::HeadItem:
            return "head_item"sv;
        case cso2::CosmeticSlot::GloveItem:
            return "glove_item"sv;
        case cso2::CosmeticSlot::BackItem:
            return "back_item"sv;
        case cso2::CosmeticSlot::StepsItem:
            return "steps_item"sv;
        case cso2::CosmeticSlot::CardItem:
            return "card_item"sv;
        case cso2::CosmeticSlot::SprayItem:
            return "spray_item"sv;
        default:
            throw std::runtime_error(
                "[GetCosmeticSlotKeyString] unknown slot used");
    }
}

inline std::string_view GetLoadoutSlotKeyString(cso2::LoadoutSlot slot)
{
    switch (slot)
    {
        case cso2::LoadoutSlot::PrimaryWeapon:
            return "primary_weapon"sv;
        case cso2::LoadoutSlot::SecondaryWeapon:
            return "secondary_weapon"sv;
        case cso2::LoadoutSlot::Melee:
            return "melee"sv;
        case cso2::LoadoutSlot::HeGrenade:
            return "hegrenade"sv;
        case cso2::LoadoutSlot::Smoke:
            return "smoke"sv;
        case cso2::LoadoutSlot::Flash:
            return "flash"sv;
        default:
            throw std::runtime_error(
                "[GetLoadoutSlotKeyString] unknown slot used");
    }
}

inline awaitable<bool> MakeUserUpdateRequest(
    asio::strand<asio::io_context::executor_type>& ioExecutor,
    tcp::resolver::results_type resolvedHost, std::uint32_t userId,
    const json::value& data)
{
    beast::tcp_stream stream(ioExecutor);
    co_await stream.async_connect(resolvedHost, use_awaitable);

    auto req = BuildPutJsonRequest(fmt::format("/users/{}", userId), data);

    co_await http::async_write(stream, req, use_awaitable);

    beast::static_buffer<256> buffer;
    http::response<http::empty_body> res;
    co_await http::async_read(stream, buffer, res, use_awaitable);

    co_return res.result() == http::status::ok;
}

UserService::UserService(std::string_view serviceHost,
                         std::string_view servicePort,
                         asio::io_context& ioContext)
    : Pingable(chrono::seconds(15), ioContext),
      m_IoExecutor(asio::make_strand(ioContext.get_executor()))
{
    this->m_Users.reserve(256);

    Log::Info("[UserService::UserService] connecting to {}:{}\n", serviceHost,
              servicePort);

    tcp::resolver resolver(ioContext);
    this->m_ResolvedHost = resolver.resolve(serviceHost, servicePort);
}

awaitable<cso2::UserPtr> UserService::GetUserById(std::uint32_t id)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        auto req = BuildGetRequest(fmt::format("/users/{}", id));

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<1024> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        if (res.result() == http::status::ok)
        {
            json::parser p;

            p.write(res.body());

            co_return this->m_Users
                .insert_or_assign(id, std::make_shared<cso2::User>(p.release()))
                .first->second;
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::GetUserById - threw '{}'\n", e.what());
    }

    co_return nullptr;
}

awaitable<cso2::InventoryPtr> UserService::GetInventory(std::uint32_t ownerId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        auto req = BuildGetRequest(fmt::format("/inventory/{}", ownerId));

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<4096> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        if (res.result() == http::status::ok)
        {
            json::parser p;

            p.write(res.body());

            co_return std::make_shared<cso2::Inventory>(p.release());
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::GetInventory - threw '{}'\n", e.what());
    }

    co_return nullptr;
}

awaitable<cso2::BuyMenuPtr> UserService::GetBuyMenu(std::uint32_t ownerId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        auto req =
            BuildGetRequest(fmt::format("/inventory/{}/buymenu", ownerId));

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<320> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        if (res.result() == http::status::ok)
        {
            json::parser p;

            p.write(res.body());

            co_return std::make_shared<cso2::BuyMenu>(p.release());
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::GetBuyMenu - threw '{}'\n", e.what());
    }

    co_return nullptr;
}

awaitable<cso2::CosmeticsPtr> UserService::GetCosmetics(std::uint32_t ownerId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        auto req =
            BuildGetRequest(fmt::format("/inventory/{}/cosmetics", ownerId));

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        if (res.result() == http::status::ok)
        {
            json::parser p;

            p.write(res.body());

            co_return std::make_shared<cso2::Cosmetics>(p.release());
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::GetCosmetics - threw '{}'\n", e.what());
    }

    co_return nullptr;
}

awaitable<cso2::LoadoutPtr> UserService::GetLoadout(std::uint32_t ownerId,
                                                    std::uint32_t loadoutNum)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        auto req = BuildGetRequest(
            fmt::format("/inventory/{}/loadout/{}", ownerId, loadoutNum));

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        if (res.result() == http::status::ok)
        {
            json::parser p;

            p.write(res.body());

            co_return std::make_shared<cso2::Loadout>(p.release());
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::GetCosmetics - threw '{}'\n", e.what());
    }

    co_return nullptr;
}

awaitable<cso2::LoadoutsArray> UserService::GetAllLoadouts(
    std::uint32_t ownerId)
{
    cso2::LoadoutsArray res;

    for (std::uint32_t i = 0; i < res.max_size(); i++)
    {
        res[i] = co_await this->GetLoadout(ownerId, i);
    }

    co_return res;
}

awaitable<std::uint32_t> UserService::Login(std::string_view username,
                                            std::string_view password)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        json::value requestData = { { "username", username },
                                    { "password", password } };
        auto req = BuildPostJsonRequest("/users/auth/login", requestData);

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        if (res.result() == http::status::ok)
        {
            json::parser p;

            p.write(res.body());

            auto jsonRes = p.release();

            co_return jsonRes.at("userId").as_int64();
        }
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::Login - threw '{}'\n", e.what());
    }

    co_return cso2::INVALID_USER_ID;
}

awaitable<bool> UserService::Logout(std::uint32_t userId)
{
    // remove the user regardless if the endpoint request fails
    this->m_Users.erase(userId);

    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        json::value requestData = { { "userId", userId } };
        auto req = BuildPostJsonRequest("/users/auth/logout", requestData);

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::empty_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        co_return res.result() == http::status::ok;
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::Logout - threw '{}'\n", e.what());
    }

    co_return false;
}

awaitable<bool> UserService::OnPing()
{
    try
    {
        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        auto req = BuildGetRequest("/ping");

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::string_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        co_return res.result() == http::status::ok;
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::OnPing - threw '{}'\n", e.what());
    }

    co_return false;
}

awaitable<bool> UserService::SetUserAvatar(std::uint32_t userId,
                                           std::uint16_t avatarId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        json::value requestData = {
            { "avatar", avatarId },
        };

        bool res = co_await MakeUserUpdateRequest(
            this->m_IoExecutor, this->m_ResolvedHost, userId, requestData);

        if (res == true)
        {
            this->m_Users[userId]->m_Avatar = avatarId;
        }

        co_return res;
    }
    catch (const std::exception& e)
    {
        Log::Warning("[UserSvc::SetUserAvatar] threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetUserSignature(std::uint32_t userId,
                                              std::string&& signature)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        std::string movedSignature = signature;

        json::value requestData = {
            { "signature", movedSignature },
        };

        bool res = co_await MakeUserUpdateRequest(
            this->m_IoExecutor, this->m_ResolvedHost, userId, requestData);

        if (res == true)
        {
            this->m_Users[userId]->m_Signature = std::move(movedSignature);
        }

        co_return res;
    }
    catch (const std::exception& e)
    {
        Log::Warning("[UserSvc::SetUserSignature] threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetUserTitle(std::uint32_t userId,
                                          std::uint16_t titleId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        json::value requestData = {
            { "title", titleId },
        };

        bool res = co_await MakeUserUpdateRequest(
            this->m_IoExecutor, this->m_ResolvedHost, userId, requestData);

        if (res == true)
        {
            this->m_Users[userId]->m_Title = titleId;
        }

        co_return res;
    }
    catch (const std::exception& e)
    {
        Log::Warning("[UserSvc::SetUserTitle] threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetUserCampaignFlags(std::uint32_t userId,
                                                  cso2::CampaignFlags flags)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        json::value requestData = {
            { "campaign_flags", std::uint32_t(flags) },
        };

        bool res = co_await MakeUserUpdateRequest(
            this->m_IoExecutor, this->m_ResolvedHost, userId, requestData);

        if (res == true)
        {
            this->m_Users[userId]->m_CampaignFlags = flags;
        }

        co_return res;
    }
    catch (const std::exception& e)
    {
        Log::Warning("[UserSvc::SetUserCampaignFlags] threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetUserStats(std::uint32_t userId,
                                          UserSvcUserStatsParams params)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        json::value requestData = {
            { "kills", params.Kills },
            { "deaths", params.Deaths },
            { "assists", params.Assists },
            { "headshots", params.Headshots },
            { "played_matches", params.MatchesPlayed },
            { "wins", params.Wins },
        };

        bool res = co_await MakeUserUpdateRequest(
            this->m_IoExecutor, this->m_ResolvedHost, userId, requestData);

        if (res == true)
        {
            this->m_Users[userId]->m_Kills = params.Kills;
            this->m_Users[userId]->m_Deaths = params.Deaths;
            this->m_Users[userId]->m_Assists = params.Assists;
            this->m_Users[userId]->m_Headshots = params.Headshots;
            this->m_Users[userId]->m_PlayedMatches = params.MatchesPlayed;
            this->m_Users[userId]->m_Wins = params.Wins;
        }

        co_return res;
    }
    catch (const std::exception& e)
    {
        Log::Warning("[UserSvc::SetUserStats] threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetBuyMenu(std::uint32_t ownerId,
                                        cso2::BuyCategoriesGroup buyMenu)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        json::value requestData = {
            { "pistols",
              json::array(buyMenu.Pistols.begin(), buyMenu.Pistols.end()) },
            { "shotguns",
              json::array(buyMenu.Shotguns.begin(), buyMenu.Shotguns.end()) },
            { "smgs", json::array(buyMenu.Smgs.begin(), buyMenu.Smgs.end()) },
            { "rifles",
              json::array(buyMenu.Rifles.begin(), buyMenu.Rifles.end()) },
            { "snipers",
              json::array(buyMenu.Snipers.begin(), buyMenu.Snipers.end()) },
            { "machineguns", json::array(buyMenu.MachineGuns.begin(),
                                         buyMenu.MachineGuns.end()) },
            { "melees",
              json::array(buyMenu.Melees.begin(), buyMenu.Melees.end()) },
            { "equipment",
              json::array(buyMenu.Equipment.begin(), buyMenu.Equipment.end()) },
        };
        auto req = BuildPutJsonRequest(
            fmt::format("/inventory/{}/buymenu", ownerId), requestData);

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::empty_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        co_return res.result() == http::status::ok;
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::SetBuyMenu - threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetCosmeticSlot(std::uint32_t ownerId,
                                             cso2::CosmeticSlot slot,
                                             std::uint32_t itemId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        json::value requestData = { { GetCosmeticSlotKeyString(slot),
                                      itemId } };
        auto req = BuildPutJsonRequest(
            fmt::format("/inventory/{}/cosmetics", ownerId), requestData);

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::empty_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        co_return res.result() == http::status::ok;
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::SetCosmeticSlot - threw '{}'\n", e.what());
    }
}

awaitable<bool> UserService::SetLoadoutWeapon(std::uint32_t ownerId,
                                              std::uint32_t loadoutNum,
                                              cso2::LoadoutSlot slot,
                                              std::uint32_t itemId)
{
    try
    {
        if (this->IsAlive() == false)
        {
            throw std::runtime_error("Service is offline");
        }

        beast::tcp_stream stream(this->m_IoExecutor);
        co_await stream.async_connect(this->m_ResolvedHost, use_awaitable);

        json::value requestData = { { GetLoadoutSlotKeyString(slot), itemId } };
        auto req = BuildPutJsonRequest(
            fmt::format("/inventory/{}/loadout/{}", ownerId, loadoutNum),
            requestData);

        co_await http::async_write(stream, req, use_awaitable);

        beast::static_buffer<256> buffer;
        http::response<http::empty_body> res;
        co_await http::async_read(stream, buffer, res, use_awaitable);

        co_return res.result() == http::status::ok;
    }
    catch (const std::exception& e)
    {
        Log::Warning("UserSvc::SetLoadoutWeapon - threw '{}'\n", e.what());
    }
}
