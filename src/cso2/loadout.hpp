#ifndef __CSO2_LOADOUT_H_
#define __CSO2_LOADOUT_H_

#include "cso2/shared.hpp"

#include <boost/json/value.hpp>

namespace json = boost::json;

namespace cso2
{
class Loadout
{
public:
    Loadout() = delete;
    Loadout(const Loadout&) = delete;

    explicit Loadout(const json::value& jv);
    ~Loadout() = default;

    [[nodiscard]] std::uint32_t GetOwnerId() const noexcept
    {
        return this->m_OwnerId;
    }

    [[nodiscard]] std::uint8_t GetLoadoutNum() const noexcept
    {
        return this->m_LoadoutNum;
    }

    [[nodiscard]] std::uint32_t GetPrimaryWeapon() const noexcept
    {
        return this->m_PrimaryWeapon;
    }

    [[nodiscard]] std::uint32_t GetSecondaryWeapon() const noexcept
    {
        return this->m_SecondaryWeapon;
    }

    [[nodiscard]] std::uint32_t GetMelee() const noexcept
    {
        return this->m_Melee;
    }

    [[nodiscard]] std::uint32_t GetHeGrenade() const noexcept
    {
        return this->m_HeGrenade;
    }

    [[nodiscard]] std::uint32_t GetFlash() const noexcept
    {
        return this->m_Flash;
    }

    [[nodiscard]] std::uint32_t GetSmoke() const noexcept
    {
        return this->m_Smoke;
    }

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
