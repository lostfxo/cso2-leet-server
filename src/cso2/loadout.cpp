#include "cso2/loadout.hpp"

#include "util/number.hpp"

namespace cso2
{
Loadout::Loadout(const json::value& jv)
{
    this->m_OwnerId = util::FastNarrow<std::uint32_t>(jv.at("owner_id").as_int64());

    this->m_LoadoutNum =
        util::FastNarrow<std::uint8_t>(jv.at("loadout_num").as_int64());
    this->m_PrimaryWeapon =
        util::FastNarrow<std::uint32_t>(jv.at("primary_weapon").as_int64());
    this->m_SecondaryWeapon =
        util::FastNarrow<std::uint32_t>(jv.at("secondary_weapon").as_int64());
    this->m_Melee = util::FastNarrow<std::uint32_t>(jv.at("melee").as_int64());
    this->m_HeGrenade =
        util::FastNarrow<std::uint32_t>(jv.at("hegrenade").as_int64());
    this->m_Flash = util::FastNarrow<std::uint32_t>(jv.at("flash").as_int64());
    this->m_Smoke = util::FastNarrow<std::uint32_t>(jv.at("smoke").as_int64());
}
}  // namespace cso2
