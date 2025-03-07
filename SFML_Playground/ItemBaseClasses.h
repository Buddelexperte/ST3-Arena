// ItemSystem.hpp
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

#include "Projectile.h"

// Enums for item usage and results
enum class ItemUse
{
    INVALID_USE = -1,
    NO_USE = 0,
    ATTACK,
    LOAD_UP,
    CANCEL_LOAD,
    PASSIVE_TRIGGER
};

enum class UseResult
{
    INVALID_USE = -1,
    FAILURE = 0,
    FAILURE_NO_RESOURCES,
    SUCCESS
};

// Base class for all items
class Item
{
public:
    struct ItemInfo
    {
        std::string name;
        std::string desc;
        ItemInfo(const std::string& n = "ItemName", const std::string& d = "Item Description")
            : name(n), desc(d)
        {}
    };

protected:
    bool bReady = false;      // Determines if the item is ready to be used
    ItemInfo info;

public:
    Item(const ItemInfo& itemInfo) : info(itemInfo) {}
    virtual ~Item() = default;

    bool isReady() const 
        { return bReady; }
    ItemInfo getItemInfo() const 
        { return info; }

    // Virtual function to "activate" the item – behavior depends on the item type.
    virtual UseResult activate(ItemUse use) = 0;
};

// =============================== WEAPON ===============================

// Weapon: active item that can perform actions (e.g., firing)
class Weapon : public Item
{
public:
    Weapon(const ItemInfo& info) 
        : Item(info)
    {
        // Weapons are considered "ready" by default
        bReady = true;
    }

    virtual ~Weapon() = default;

    // In this basic example, only ATTACK is supported.
    virtual UseResult activate(ItemUse use) override
    {
        if (use == ItemUse::ATTACK)
        {
            std::cout << "Weapon \"" << info.name << "\" fired.\n";
            // Add additional firing logic (ammo check, cooldowns, etc.) here.
            return UseResult::SUCCESS;
        }
        return UseResult::FAILURE;
    }

    // Additional weapon-specific methods could be added (e.g., reload, update)
};

// =============================== PERK ===============================

// Enum for various perk triggers.
enum class PerkTrigger
{
    OnEnemyContact,
    OnPlayerDamaged,
    OnWeaponShot,
    OnWeaponHit
    // Extend with additional triggers as needed.
};

// Perk: passive boost that triggers on specific game events.
class Perk : public Item
{
protected:
    // Each perk can listen for one or more triggers.
    std::vector<PerkTrigger> triggers;

public:
    Perk(const ItemInfo& info, const std::vector<PerkTrigger>& triggerList)
        : Item(info), triggers(triggerList)
    {
        // Perks are always active/passive once acquired.
        bReady = true;
    }

    virtual ~Perk() = default;

    // While not "activated" directly like a weapon, we provide a stub for interface consistency.
    virtual UseResult activate(ItemUse use) override
    {
        if (use == ItemUse::PASSIVE_TRIGGER)
        {
            // This function could log or perform a simple activation if needed.
            return UseResult::SUCCESS;
        }
        return UseResult::FAILURE;
    }

    // Called when a game event occurs; checks if the perk should trigger.
    void onTrigger(PerkTrigger trigger)
    {
        if (std::find(triggers.begin(), triggers.end(), trigger) != triggers.end())
        {
            std::cout << "Perk \"" << info.name << "\" triggered on event: ";
            switch (trigger)
            {
            case PerkTrigger::OnEnemyContact: std::cout << "OnEnemyContact"; break;
            case PerkTrigger::OnPlayerDamaged: std::cout << "OnPlayerDamaged"; break;
            case PerkTrigger::OnWeaponShot:    std::cout << "OnWeaponShot"; break;
            case PerkTrigger::OnWeaponHit:     std::cout << "OnWeaponHit"; break;
            default: std::cout << "Unknown Trigger"; break;
            }
            std::cout << "\n";
            // Insert perk effect logic here (e.g., boost stats, activate shield, etc.)
        }
    }
};