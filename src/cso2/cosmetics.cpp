#include "cso2/cosmetics.hpp"

#include "util/number.hpp"

namespace cso2
{
Cosmetics::Cosmetics(const json::value& jv)
{
    this->m_OwnerId = util::FastNarrow<std::uint32_t>(jv.at("owner_id").as_int64());

    this->m_CtItem = util::FastNarrow<std::uint32_t>(jv.at("ct_item").as_int64());
    this->m_TerItem = util::FastNarrow<std::uint32_t>(jv.at("ter_item").as_int64());
    this->m_HeadItem =
        util::FastNarrow<std::uint32_t>(jv.at("head_item").as_int64());
    this->m_GloveItem =
        util::FastNarrow<std::uint32_t>(jv.at("glove_item").as_int64());
    this->m_BackItem =
        util::FastNarrow<std::uint32_t>(jv.at("back_item").as_int64());
    this->m_StepsItem =
        util::FastNarrow<std::uint32_t>(jv.at("steps_item").as_int64());
    this->m_CardItem =
        util::FastNarrow<std::uint32_t>(jv.at("card_item").as_int64());
    this->m_SprayItem =
        util::FastNarrow<std::uint32_t>(jv.at("spray_item").as_int64());
}
}  // namespace cso2
