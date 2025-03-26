// ItemSystem.hpp
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

#include "Projectile.h"
#include "Functions.h"
#include "ValueBar.h"

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
    FAILURE_COOLDOWN,
    FAILURE_NO_RESOURCES,
    SUCCESS
};

class Inventory; // Forward declaration

// Base class for all items
class Item : public IMovable
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
    Inventory* owningInventory = nullptr;
    bool bReady = false;      // Determines if the item is ready to be used
    ItemInfo info;

public:
    Item(const ItemInfo& itemInfo) : info(itemInfo) {}
    virtual ~Item() = default;

    void setOwningInventory(Inventory* newInv)
        { owningInventory = newInv; }

    Inventory* getOwningInventory()
        { return owningInventory; }

    bool isReady() const 
        { return bReady; }
    
    ItemInfo getItemInfo() const 
        { return info; }

    // Virtual function to "activate" the item – behavior depends on the item type.
    virtual UseResult activate(const ItemUse& use) = 0;

    virtual void tick(const float& deltaTime)
        { }
};

// =============================== WEAPON ===============================

// Weapon: active item that can perform actions (e.g., firing)
class Weapon : public Item
{
protected:
	float damage = 0.0f;    // Base damage of the weapon
    std::unique_ptr<ValueBar> cooldown;
    std::unique_ptr<ProjectileSpawner> projSpawner;
public:
    Weapon(const ItemInfo& info, const float& damage, std::unique_ptr<ProjectileSpawner> ps, std::unique_ptr<ValueBar> cd) 
        : 
        Item(info),
        damage(damage),
        projSpawner(std::move(ps)),
        cooldown(std::move(cd))
    {
        cooldown->setValue(cooldown->getMaxValue() + 0.5f);
        // Weapons are considered "ready" by default
        bReady = true;
    }

    virtual ~Weapon() = default;

    virtual UseResult activate(const ItemUse& use) override;

    virtual void tick(const float& deltaTime) override;

    // Render Info extension for ProjectileSpawners
    void setPosition(const sf::Vector2f& newPos) override
    {
        IMovable::setPosition(newPos);
        projSpawner->setPosition(newPos);
    }

    void addPosition(const sf::Vector2f& delta) override
    {
        IMovable::addPosition(delta);
        projSpawner->addPosition(delta);
    }

    void setRotation(const float& newRot) override
    {
        IMovable::setRotation(newRot);
        projSpawner->setRotation(newRot);
    }

    void setSize(const sf::Vector2f& newSize) override
    {
        IMovable::setSize(newSize);
        projSpawner->setSize(newSize);
    }

    void setRenderInfo(const RenderInfo& newRenderInfo) override
    {
        IMovable::setRenderInfo(newRenderInfo);
        projSpawner->setRenderInfo(newRenderInfo);
    }

    float getDamage() const
    {
        return damage;
    }

	void setDamage(const float& newDamage)
	{
		damage = newDamage;
	}

    // Additional weapon-specific methods could be added (e.g., reload, update)
};

std::unique_ptr<Weapon> makeWeapon(std::string weaponName);

// =============================== PERK ===============================

// Enum for various perk triggers.
enum class PerkTrigger
{
    OnEnemyContact,
    OnPlayerDamaged,
    OnWeaponShot,
    OnEnemyHit
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

    virtual UseResult activate(const ItemUse& use) override
    {
        if (use == ItemUse::PASSIVE_TRIGGER)
        {
            return UseResult::SUCCESS;
        }
        return UseResult::FAILURE;
    }

    // Called when a game event occurs; checks if the perk should trigger.
    void onTrigger(PerkTrigger trigger)
    {
        if (vectorContains(triggers, trigger))
        {
            std::cout << "Perk \"" << info.name << "\" triggered on event: ";
            switch (trigger)
            {
            case PerkTrigger::OnEnemyContact:
                std::cout << "OnEnemyContact";
                break;
            case PerkTrigger::OnPlayerDamaged:
                std::cout << "OnPlayerDamaged";
                break;
            case PerkTrigger::OnWeaponShot:
                std::cout << "OnWeaponShot";
                break;
            case PerkTrigger::OnEnemyHit:     
                std::cout << "OnWeaponHit";
                break;
            default: 
                std::cout << "Unknown Trigger";
                break;
            }
            std::cout << "\n";
            // Insert perk effect logic here (e.g., boost stats, activate shield, etc.)
        }
    }
};