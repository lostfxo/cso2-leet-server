#include "cso2/inventory.hpp"

#include <boost/json/value.hpp>
#include <iterator>

#include "util/number.hpp"

namespace cso2
{
Inventory::Inventory(const boost::json::value& jv)
{
    this->m_OwnerId =
        util::FastNarrow<std::uint32_t>(jv.at("owner_id").as_int64());

    const auto& items = jv.at("items").as_array();
    this->m_Items.reserve(items.size());

    std::transform(
        items.begin(), items.end(), std::back_inserter(this->m_Items),
        [](const boost::json::value& v) -> InventoryItem
        {
            const auto& obj = v.as_object();
            return { static_cast<std::uint32_t>(obj.at("item_id").as_int64()),
                     static_cast<std::uint16_t>(obj.at("ammount").as_int64()) };
        });
}
}  // namespace cso2
