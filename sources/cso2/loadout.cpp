#include "cso2/loadout.hpp"

#include <gsl/gsl>

namespace cso2
{
Loadout::Loadout(const json::value& jv)
{
    this->m_OwnerId = gsl::narrow<std::uint32_t>(jv.at("owner_id").as_int64());

    this->m_LoadoutNum =
        gsl::narrow<std::uint8_t>(jv.at("loadout_num").as_int64());
    this->m_PrimaryWeapon =
        gsl::narrow<std::uint32_t>(jv.at("primary_weapon").as_int64());
    this->m_SecondaryWeapon =
        gsl::narrow<std::uint32_t>(jv.at("secondary_weapon").as_int64());
    this->m_Melee = gsl::narrow<std::uint32_t>(jv.at("melee").as_int64());
    this->m_HeGrenade =
        gsl::narrow<std::uint32_t>(jv.at("hegrenade").as_int64());
    this->m_Flash = gsl::narrow<std::uint32_t>(jv.at("flash").as_int64());
    this->m_Smoke = gsl::narrow<std::uint32_t>(jv.at("smoke").as_int64());
}
}  // namespace cso2
