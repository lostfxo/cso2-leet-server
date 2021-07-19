#ifndef __CSO2_COSMETICS_H_
#define __CSO2_COSMETICS_H_

#include "cso2/shared.hpp"

#include "util/json_fwd.hpp"

namespace cso2
{
class Cosmetics
{
public:
    explicit Cosmetics(const boost::json::value& jv);
    ~Cosmetics() = default;

    std::uint32_t GetOwnerId() const { return this->m_OwnerId; }

    std::uint32_t GetCtItem() const { return this->m_CtItem; }
    std::uint32_t GetTerItem() const { return this->m_TerItem; }
    std::uint32_t GetHeadItem() const { return this->m_HeadItem; }
    std::uint32_t GetGloveItem() const { return this->m_GloveItem; }
    std::uint32_t GetBackItem() const { return this->m_BackItem; }
    std::uint32_t GetStepsItem() const { return this->m_StepsItem; }
    std::uint32_t GetCardItem() const { return this->m_CardItem; }
    std::uint32_t GetSprayItem() const { return this->m_SprayItem; }

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
