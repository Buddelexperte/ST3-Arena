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