#ifndef __CSO2_INVENTORY_H_
#define __CSO2_INVENTORY_H_

#include "cso2/shared.hpp"
#include "util/json_fwd.hpp"

namespace cso2
{
class Inventory
{
public:
    explicit Inventory(const boost::json::value& jv);
    ~Inventory() = default;

    std::uint32_t GetOwnerId() const { return this->m_OwnerId; }
    const InventoryItemsArray& GetItems() const { return this->m_Items; }

private:
    std::uint32_t m_OwnerId;
    InventoryItemsArray m_Items;
};

}  // namespace cso2

#endif  // __CSO2_INVENTORY_H_
