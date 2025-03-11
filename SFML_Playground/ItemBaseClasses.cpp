#pragma once

#include "ItemBaseClasses.h"
#include "Inventory.h"

UseResult Weapon::activate(const ItemUse& use)
{
    switch (use)
    {
    case ItemUse::NO_USE:
        break;
    case ItemUse::ATTACK:
        if (cooldownLeft > 0.0f)
        {
            return UseResult::FAILURE_COOLDOWN;
        }

        std::cout << "Weapon \"" << info.name << "\" fired.\n";
        // Add additional firing logic (ammo check, cooldowns, etc.) here.
        cooldownLeft = getMaxCooldown() - owningInventory->getCooldownSubtractor();
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

    return UseResult::FAILURE;
}

void Weapon::tick(const float& deltaTime)
{
    cooldownLeft -= deltaTime * owningInventory->getCooldownMultiplier();
}