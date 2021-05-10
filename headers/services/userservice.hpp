#ifndef __USERSERVICE_H_
#define __USERSERVICE_H_

#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_map>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>

#include "generic/pingable.hpp"

#include "cso2/definitions.hpp"
#include "cso2/shared.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

struct UserSvcUserStatsParams
{
    std::uint32_t Kills;
    std::uint32_t Deaths;
    std::uint32_t Assists;
    std::uint32_t Headshots;
    std::uint32_t MatchesPlayed;
    std::uint32_t Wins;
};

class UserService final : public Pingable
{
public:
    UserService(std::string_view serviceHost, std::string_view servicePort,
                asio::io_context& ioContext);

    [[nodiscard]] awaitable<cso2::UserPtr> GetUserById(std::uint32_t id);
    [[nodiscard]] awaitable<std::uint32_t> Login(std::string_view username,
                                                 std::string_view password);
    awaitable<bool> Logout(std::uint32_t userId);

    [[nodiscard]] awaitable<cso2::InventoryPtr> GetInventory(
        std::uint32_t ownerId);
    [[nodiscard]] awaitable<cso2::BuyMenuPtr> GetBuyMenu(std::uint32_t ownerId);
    [[nodiscard]] awaitable<cso2::CosmeticsPtr> GetCosmetics(
        std::uint32_t ownerId);
    [[nodiscard]] awaitable<cso2::LoadoutPtr> GetLoadout(
        std::uint32_t ownerId, std::uint32_t loadoutNum);
    [[nodiscard]] awaitable<cso2::LoadoutsArray> GetAllLoadouts(
        std::uint32_t ownerId);

    awaitable<bool> SetUserAvatar(std::uint32_t userId, std::uint16_t avatarId);
    awaitable<bool> SetUserSignature(std::uint32_t userId,
                                     std::string&& signature);
    awaitable<bool> SetUserTitle(std::uint32_t userId, std::uint16_t titleId);

    awaitable<bool> SetUserCampaignFlags(std::uint32_t userId,
                                         cso2::CampaignFlags flags);
    awaitable<bool> SetUserStats(std::uint32_t userId,
                                 UserSvcUserStatsParams params);

    awaitable<bool> SetBuyMenu(std::uint32_t ownerId,
                               cso2::BuyCategoriesGroup buyMenu);

    awaitable<bool> SetCosmeticSlot(std::uint32_t ownerId,
                                    cso2::CosmeticSlot slot,
                                    std::uint32_t itemId);

    awaitable<bool> SetLoadoutWeapon(std::uint32_t ownerId,
                                     std::uint32_t loadoutNum,
                                     cso2::LoadoutSlot slot,
                                     std::uint32_t itemId);

protected:
    virtual awaitable<bool> OnPing() override;

    virtual std::string_view GetServiceName() override
    {
        return "UserService";
    };

private:
    asio::strand<asio::io_context::executor_type> m_IoExecutor;
    tcp::resolver::results_type m_ResolvedHost;

    std::unordered_map<std::uint32_t, cso2::UserPtr> m_Users;
};

extern std::unique_ptr<UserService> g_UserService;

#endif  // __USERSERVICE_H_
