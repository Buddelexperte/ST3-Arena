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

#include "AllWeapons.h"
#include "Functions.h"

std::unique_ptr<Weapon> makeWeapon(std::string weaponName)
{
    weaponName = toUpperCase(weaponName);

    if (weaponName == "PISTOL") {
        return std::make_unique<Pistol>();
    }
    else if (weaponName == "RIFLE") {
        return std::make_unique<Rifle>();
    }
    else if (weaponName == "RIFLE BURST" || weaponName == "RIFLE_BURST") {
        return std::make_unique<Rifle_Burst>();
    }
    else if (weaponName == "SHOTGUN") {
        return std::make_unique<Shotgun>();
    }
}
