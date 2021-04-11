#ifndef __CSO2_BUYMENU_H_
#define __CSO2_BUYMENU_H_

#include "cso2/shared.hpp"

#include <boost/json/value.hpp>

namespace json = boost::json;

namespace cso2
{
class BuyMenu
{
public:
    BuyMenu() = delete;
    BuyMenu(const BuyMenu&) = delete;

    explicit BuyMenu(const json::value& jv);
    ~BuyMenu() = default;

    [[nodiscard]] std::uint32_t GetOwnerId() const noexcept
    {
        return this->m_OwnerId;
    }

    [[nodiscard]] const BuyCategory& GetPistols() const noexcept
    {
        return this->m_Pistols;
    }
    [[nodiscard]] const BuyCategory& GetShotguns() const noexcept
    {
        return this->m_Shotguns;
    }
    [[nodiscard]] const BuyCategory& GetSmgs() const noexcept
    {
        return this->m_Smgs;
    }
    [[nodiscard]] const BuyCategory& GetRifles() const noexcept
    {
        return this->m_Rifles;
    }
    [[nodiscard]] const BuyCategory& GetSnipers() const noexcept
    {
        return this->m_Snipers;
    }
    [[nodiscard]] const BuyCategory& GetMachineGuns() const noexcept
    {
        return this->m_MachineGuns;
    }
    [[nodiscard]] const BuyCategory& GetMelees() const noexcept
    {
        return this->m_Melees;
    }
    [[nodiscard]] const BuyCategory& GetEquipment() const noexcept
    {
        return this->m_Equipment;
    }

private:
    std::uint32_t m_OwnerId;

    BuyCategory m_Pistols;
    BuyCategory m_Shotguns;
    BuyCategory m_Smgs;
    BuyCategory m_Rifles;
    BuyCategory m_Snipers;
    BuyCategory m_MachineGuns;
    BuyCategory m_Melees;
    BuyCategory m_Equipment;
};

}  // namespace cso2

#endif  // __CSO2_BUYMENU_H_
