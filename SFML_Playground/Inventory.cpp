#pragma once

#include "Inventory.h" // Own header file
#include "Player.h"
#include "GameInstance.h"

Inventory::Inventory(std::unique_ptr<Weapon> initialWeapon)
    : Inventory()
{
    owner = GI_Arena::getInstance().getPlayer(); 
    addWeapon(std::move(initialWeapon));
}

// Adds a new weapon. If this is the first weapon, it becomes the active weapon.
void Inventory::addWeapon(std::unique_ptr<Weapon> weapon)
{
    if (!weapon)
        return;
    
    weapon->setOwningInventory(this);
    weapons.push_back(std::move(weapon));
    if (activeWeaponIndex == -1)
        activeWeaponIndex = 0;
}

// Switches the active weapon by its index.
bool Inventory::selectWeapon(size_t index)
{
    if (index < weapons.size())
    {
        activeWeaponIndex = static_cast<int>(index);
        std::cout << "Switched active weapon to: "
            << weapons[activeWeaponIndex]->getItemInfo().name << "\n";
        weapons[activeWeaponIndex]->setRenderInfo(owner->getRenderInfo());
        return true;
    }
    return false;
}

// Returns a pointer to the current active weapon.
Weapon* Inventory::getActiveWeapon() const
{
    if (activeWeaponIndex >= 0 && activeWeaponIndex < static_cast<int>(weapons.size()))
        return weapons[activeWeaponIndex].get();
    return nullptr;
}

// Adds a new perk.
void Inventory::addPerk(std::unique_ptr<Perk> perk)
{
    if (!perk)
        return;

    perk->setOwningInventory(this);
    perks.push_back(std::move(perk));
}

// This function should be called when an in-game event occurs to trigger all relevant perks.
void Inventory::triggerPerks(PerkTriggerInfo& triggerInfo)
{
    for (auto& perk : perks)
    {
        perk->onTrigger(triggerInfo);
    }
}

// Removes a weapon by index and returns the unique pointer.
std::unique_ptr<Weapon> Inventory::removeWeapon(size_t index)
{
    if (index >= weapons.size())
        return nullptr;

    // Move the unique_ptr out of the vector.
    auto removedWeapon = std::move(weapons[index]);
    weapons.erase(weapons.begin() + index);

    // Update the active weapon index.
    if (activeWeaponIndex == static_cast<int>(index))
    {
        activeWeaponIndex = weapons.empty() ? -1 : 0;
    }
    else if (activeWeaponIndex > static_cast<int>(index))
    {
        activeWeaponIndex--;
    }
    return removedWeapon;
}

// Removes a perk by index and returns the unique pointer.
std::unique_ptr<Perk> Inventory::removePerk(size_t index)
{
    if (index >= perks.size())
        return nullptr;

    // Move the unique_ptr out of the vector.
    auto removedPerk = std::move(perks[index]);
    perks.erase(perks.begin() + index);
    return removedPerk;
}

void Inventory::reset()
{
    clear();
    // Get wepaon by StartWeaponName
    std::unique_ptr<Weapon> startWeapon = makeWeapon(startWeaponName);
    addWeapon(std::move(startWeapon));
}

// Optional: an update function to process inventory-related logic each frame.
void Inventory::update(const float& deltaTime)
{
    // For example: check if weapons are reloading or if perk effects need updating.
}