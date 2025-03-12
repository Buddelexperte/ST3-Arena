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
        {
        if (cooldown->isNotEmpty())
            return UseResult::FAILURE_COOLDOWN;

        std::cout << "Weapon \"" << info.name << "\" fired.\n";
        // Add additional firing logic (ammo check, cooldowns, etc.) here.
        float newCooldownLeft = cooldown->getMaxValue() - owningInventory->getCooldownSubtractor();
        cooldown->setValue(newCooldownLeft);
        }
        break;
    case ItemUse::LOAD_UP:
        break;
    case ItemUse::CANCEL_LOAD:
        break;
    case ItemUse::PASSIVE_TRIGGER:
        break;
    case ItemUse::INVALID_USE: default:
        return UseResult::FAILURE;
        break;
    }

    return UseResult::SUCCESS;
}

void Weapon::tick(const float& deltaTime)
{
    float cooldownDelta = -deltaTime * owningInventory->getCooldownMultiplier();
    cooldown->addValue(cooldownDelta);
}