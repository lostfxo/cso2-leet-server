#include "definitions.hpp"

namespace cso2
{
std::string_view GetCosmeticSlotKeyString(cso2::CosmeticSlot slot)
{
    switch (slot)
    {
        case cso2::CosmeticSlot::CtItem:
            return "ct_item"sv;
        case cso2::CosmeticSlot::TerItem:
            return "ter_item"sv;
        case cso2::CosmeticSlot::HeadItem:
            return "head_item"sv;
        case cso2::CosmeticSlot::GloveItem:
            return "glove_item"sv;
        case cso2::CosmeticSlot::BackItem:
            return "back_item"sv;
        case cso2::CosmeticSlot::StepsItem:
            return "steps_item"sv;
        case cso2::CosmeticSlot::CardItem:
            return "card_item"sv;
        case cso2::CosmeticSlot::SprayItem:
            return "spray_item"sv;
    }

    return "(invalid)"sv;
}

std::string_view GetLoadoutSlotKeyString(cso2::LoadoutSlot slot)
{
    switch (slot)
    {
        case cso2::LoadoutSlot::PrimaryWeapon:
            return "primary_weapon"sv;
        case cso2::LoadoutSlot::SecondaryWeapon:
            return "secondary_weapon"sv;
        case cso2::LoadoutSlot::Melee:
            return "melee"sv;
        case cso2::LoadoutSlot::HeGrenade:
            return "hegrenade"sv;
        case cso2::LoadoutSlot::Smoke:
            return "smoke"sv;
        case cso2::LoadoutSlot::Flash:
            return "flash"sv;
    }

    return "(invalid)"sv;
}
}  // namespace cso2
