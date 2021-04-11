#ifndef __CSO2_SHARED_H_
#define __CSO2_SHARED_H_

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

namespace cso2
{
constexpr const std::uint32_t INVALID_USER_ID = 0;
constexpr const std::uint32_t MAX_BUY_CATEGORY_ITEMS = 9;
constexpr const std::uint32_t MAX_LOADOUTS_NUM = 3;
constexpr const std::uint32_t MAX_LOADOUT_ITEMS_NUM = 16;
constexpr const std::uint32_t UNAUTHORISED_USER_ID = std::uint32_t(-1);

class BuyMenu;
class Cosmetics;
class Inventory;
class Loadout;
class User;

using BuyCategory = std::array<std::uint32_t, MAX_BUY_CATEGORY_ITEMS>;
using BuyMenuPtr = std::shared_ptr<BuyMenu>;
using CosmeticsPtr = std::shared_ptr<Cosmetics>;
using InventoryPtr = std::shared_ptr<Inventory>;
using LoadoutPtr = std::shared_ptr<Loadout>;
using LoadoutsArray = std::array<LoadoutPtr, MAX_LOADOUTS_NUM>;
using UserPtr = std::shared_ptr<User>;
using UserId_t = std::uint32_t;

struct InventoryItem
{
    std::uint32_t ItemId;
    std::uint16_t Ammount;
};

using InventoryItemsArray = std::vector<InventoryItem>;

struct BuyCategoriesGroup
{
    cso2::BuyCategory Pistols;
    cso2::BuyCategory Shotguns;
    cso2::BuyCategory Smgs;
    cso2::BuyCategory Rifles;
    cso2::BuyCategory Snipers;
    cso2::BuyCategory MachineGuns;
    cso2::BuyCategory Melees;
    cso2::BuyCategory Equipment;
};
}  // namespace cso2

#endif  // __CSO2_SHARED_H_
