#ifndef __CSO2_UNLOCK_TYPES_
#define __CSO2_UNLOCK_TYPES_

#include <cstdint>

namespace cso2
{
enum class UnlockCurrency
{
    Points = 0,
    Mileage = 1
};

struct UnlockItem
{
    std::uint32_t ItemId;
    std::uint32_t Price;
    UnlockCurrency Currency;
};

struct UnlockProgress
{
    std::uint32_t TargetItemId;
    std::uint32_t ParentItemId;
    std::uint32_t Kills;
};
}  // namespace cso2

#endif  // __CSO2_UNLOCK_TYPES_
