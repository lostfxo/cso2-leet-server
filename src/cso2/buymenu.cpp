#include "cso2/buymenu.hpp"

#include "util/number.hpp"

namespace cso2
{
BuyMenu::BuyMenu(const json::value& jv)
{
    this->m_OwnerId = util::FastNarrow<std::uint32_t>(jv.at("owner_id").as_int64());

    auto arrayTransform = [](const json::value v) -> std::uint32_t {
        return util::FastNarrow<std::uint32_t>(v.as_int64());
    };

    const auto& pistols = jv.at("pistols").as_array();
    std::transform(pistols.begin(), pistols.end(), this->m_Pistols.begin(),
                   arrayTransform);

    const auto& shotguns = jv.at("shotguns").as_array();
    std::transform(shotguns.begin(), shotguns.end(), this->m_Shotguns.begin(),
                   arrayTransform);

    const auto& smgs = jv.at("smgs").as_array();
    std::transform(smgs.begin(), smgs.end(), this->m_Smgs.begin(),
                   arrayTransform);

    const auto& rifles = jv.at("rifles").as_array();
    std::transform(rifles.begin(), rifles.end(), this->m_Rifles.begin(),
                   arrayTransform);

    const auto& snipers = jv.at("snipers").as_array();
    std::transform(snipers.begin(), snipers.end(), this->m_Snipers.begin(),
                   arrayTransform);

    const auto& machineguns = jv.at("machineguns").as_array();
    std::transform(machineguns.begin(), machineguns.end(),
                   this->m_MachineGuns.begin(), arrayTransform);

    const auto& melees = jv.at("melees").as_array();
    std::transform(melees.begin(), melees.end(), this->m_Melees.begin(),
                   arrayTransform);

    const auto& equipment = jv.at("equipment").as_array();
    std::transform(equipment.begin(), equipment.end(),
                   this->m_Equipment.begin(), arrayTransform);
}
}  // namespace cso2
