#include "cso2/cosmetics.hpp"

#include <gsl/gsl>

namespace cso2
{
Cosmetics::Cosmetics(const json::value& jv)
{
    this->m_OwnerId = gsl::narrow<std::uint32_t>(jv.at("owner_id").as_int64());

    this->m_CtItem = gsl::narrow<std::uint32_t>(jv.at("ct_item").as_int64());
    this->m_TerItem = gsl::narrow<std::uint32_t>(jv.at("ter_item").as_int64());
    this->m_HeadItem =
        gsl::narrow<std::uint32_t>(jv.at("head_item").as_int64());
    this->m_GloveItem =
        gsl::narrow<std::uint32_t>(jv.at("glove_item").as_int64());
    this->m_BackItem =
        gsl::narrow<std::uint32_t>(jv.at("back_item").as_int64());
    this->m_StepsItem =
        gsl::narrow<std::uint32_t>(jv.at("steps_item").as_int64());
    this->m_CardItem =
        gsl::narrow<std::uint32_t>(jv.at("card_item").as_int64());
    this->m_SprayItem =
        gsl::narrow<std::uint32_t>(jv.at("spray_item").as_int64());
}
}  // namespace cso2
