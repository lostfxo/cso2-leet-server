#ifndef __CSO2_BUYMENU_H_
#define __CSO2_BUYMENU_H_

#include "cso2/shared.hpp"
#include "util/json_fwd.hpp"

namespace cso2
{
class BuyMenu
{
public:
    explicit BuyMenu(const boost::json::value& jv);
    ~BuyMenu() = default;

    std::uint32_t GetOwnerId() const { return this->m_OwnerId; }

    const BuyCategory& GetPistols() const { return this->m_Pistols; }
    const BuyCategory& GetShotguns() const { return this->m_Shotguns; }
    const BuyCategory& GetSmgs() const { return this->m_Smgs; }
    const BuyCategory& GetRifles() const { return this->m_Rifles; }
    const BuyCategory& GetSnipers() const { return this->m_Snipers; }
    const BuyCategory& GetMachineGuns() const { return this->m_MachineGuns; }
    const BuyCategory& GetMelees() const { return this->m_Melees; }
    const BuyCategory& GetEquipment() const { return this->m_Equipment; }

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
