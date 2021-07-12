#ifndef __CSO2_INVENTORY_H_
#define __CSO2_INVENTORY_H_

#include "cso2/shared.hpp"

#include <boost/json/value.hpp>

namespace json = boost::json;

namespace cso2
{
class Inventory
{
public:
    Inventory() = delete;
    Inventory(const Inventory&) = delete;

    explicit Inventory(const json::value& jv);
    ~Inventory() = default;

    [[nodiscard]] std::uint32_t GetOwnerId() const noexcept
    {
        return this->m_OwnerId;
    }

    [[nodiscard]] const InventoryItemsArray& GetItems() const noexcept
    {
        return this->m_Items;
    }

private:
    std::uint32_t m_OwnerId;
    InventoryItemsArray m_Items;
};

}  // namespace cso2

#endif  // __CSO2_INVENTORY_H_
