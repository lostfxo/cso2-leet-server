#ifndef __CSO2_LOADOUT_H_
#define __CSO2_LOADOUT_H_

#include "cso2/shared.hpp"
#include "util/json_fwd.hpp"

namespace cso2
{
class Loadout
{
public:
    explicit Loadout(const boost::json::value& jv);
    ~Loadout() = default;

    std::uint32_t GetOwnerId() const { return this->m_OwnerId; }
    std::uint8_t GetLoadoutNum() const { return this->m_LoadoutNum; }
    std::uint32_t GetPrimaryWeapon() const { return this->m_PrimaryWeapon; }
    std::uint32_t GetSecondaryWeapon() const { return this->m_SecondaryWeapon; }
    std::uint32_t GetMelee() const { return this->m_Melee; }
    std::uint32_t GetHeGrenade() const { return this->m_HeGrenade; }
    std::uint32_t GetFlash() const { return this->m_Flash; }
    std::uint32_t GetSmoke() const { return this->m_Smoke; }

private:
    // members are mem aligned
    std::uint32_t m_OwnerId;

    std::uint32_t m_PrimaryWeapon;
    std::uint32_t m_SecondaryWeapon;
    std::uint32_t m_Melee;
    std::uint32_t m_HeGrenade;
    std::uint32_t m_Flash;
    std::uint32_t m_Smoke;

    std::uint8_t m_LoadoutNum;
};
}  // namespace cso2

#endif  // __CSO2_LOADOUT_H_
