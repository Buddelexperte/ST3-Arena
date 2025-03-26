#pragma once

#include "ItemBaseClasses.h"
#include "Inventory.h"

// TODO: Make Weapon, enemy and other attributes read from a file

UseResult Weapon::activate(const ItemUse& use)
{
    switch (use)
    {
    case ItemUse::NO_USE:
        break;
    case ItemUse::ATTACK:
        {
            if (cooldown->isNotEmpty())
            {
                return UseResult::FAILURE_COOLDOWN;
            }
            // Add additional firing logic (ammo check, cooldowns, etc.) here.
            float cooldownDeficit = owningInventory->getCooldownSubtractor();
            cooldown->reset(cooldownDeficit);
        }
        return UseResult::SUCCESS;
        break;
    case ItemUse::LOAD_UP:
        break;
    case ItemUse::CANCEL_LOAD:
        break;
    case ItemUse::PASSIVE_TRIGGER:
        break;
    case ItemUse::INVALID_USE: default:
        break;
    }

    return UseResult::INVALID_USE;
}

void Weapon::tick(const float& deltaTime)
{
    float cooldownDelta = -deltaTime * owningInventory->getCooldownMultiplier();
    cooldown->addValue(cooldownDelta);

	activate(ItemUse::NO_USE);
}


// Weapon Factory
#include "AllWeapons.h"

const std::vector<StringWeapon> weaponMappings = {
    {"PISTOL", PISTOL},
    {"RIFLE", RIFLE},
    {"SHOTGUN", SHOTGUN},
    {"BURST RIFLE", BURST_RIFLE},
    {"RIFLE BURST", BURST_RIFLE},
    {"RIFLE_BURST", BURST_RIFLE}
};


WeaponType getWeaponTypeFromText(const std::string& buttonText)
{
    std::string normalized = normalizeWeaponName(buttonText);
    // For simplicity, perform a linear search; for many entries, consider using a hash map.
    for (const auto& mapping : weaponMappings)
    {
        if (normalized == mapping.name)
        {
            return mapping.type;
        }
    }
    return UNKNOWN_WEAPON;
}
std::unique_ptr<Weapon> makeWeapon(const std::string& selectWeaponText)
{
    const WeaponType type = getWeaponTypeFromText(selectWeaponText);

    switch (type) {
    case PISTOL:
        return std::make_unique<Pistol>();
    case RIFLE:
        return std::make_unique<Rifle>();
    case SHOTGUN:
        return std::make_unique<Shotgun>();
    case BURST_RIFLE:
        return std::make_unique<Rifle_Burst>();
    default:
        break;
    }

    return nullptr;
}
