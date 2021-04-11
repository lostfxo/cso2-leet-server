#ifndef __CSO2_COSMETICS_H_
#define __CSO2_COSMETICS_H_

#include "cso2/shared.hpp"

#include <boost/json/value.hpp>

namespace json = boost::json;

namespace cso2
{
class Cosmetics
{
public:
    Cosmetics() = delete;
    Cosmetics(const Cosmetics&) = delete;

    explicit Cosmetics(const json::value& jv);
    ~Cosmetics() = default;

    [[nodiscard]] std::uint32_t GetOwnerId() const noexcept
    {
        return this->m_OwnerId;
    }

    [[nodiscard]] std::uint32_t GetCtItem() const noexcept { return this->m_CtItem; }
    [[nodiscard]] std::uint32_t GetTerItem() const noexcept
    {
        return this->m_TerItem;
    }
    [[nodiscard]] std::uint32_t GetHeadItem() const noexcept
    {
        return this->m_HeadItem;
    }
    [[nodiscard]] std::uint32_t GetGloveItem() const noexcept
    {
        return this->m_GloveItem;
    }
    [[nodiscard]] std::uint32_t GetBackItem() const noexcept
    {
        return this->m_BackItem;
    }
    [[nodiscard]] std::uint32_t GetStepsItem() const noexcept
    {
        return this->m_StepsItem;
    }
    [[nodiscard]] std::uint32_t GetCardItem() const noexcept
    {
        return this->m_CardItem;
    }
    [[nodiscard]] std::uint32_t GetSprayItem() const noexcept
    {
        return this->m_SprayItem;
    }

private:
    std::uint32_t m_OwnerId;

    std::uint32_t m_CtItem;
    std::uint32_t m_TerItem;
    std::uint32_t m_HeadItem;
    std::uint32_t m_GloveItem;
    std::uint32_t m_BackItem;
    std::uint32_t m_StepsItem;
    std::uint32_t m_CardItem;
    std::uint32_t m_SprayItem;
};
}  // namespace cso2

#endif  // __CSO2_COSMETICS_H_
