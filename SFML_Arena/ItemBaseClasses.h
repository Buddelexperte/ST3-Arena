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
    PASSIVE_TRIGGER,
    ON_EQUIP
};

enum class UseResult
{
    INVALID_USE = -1,
    FAILURE = 0,
    FAILURE_COOLDOWN,
    FAILURE_NO_RESOURCES,
    SUCCESS,
    CRITICAL_SUCCESS
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

enum WeaponType {
    UNKNOWN_WEAPON = -1,
    PISTOL = 0,
    RIFLE,
    SHOTGUN,
    BURST_RIFLE,
};

// Mapping structure for weapon names to WeaponType
struct StringWeapon
{
    std::string name;
    WeaponType type;
};

std::string normalizeWeaponName(const std::string& name);

// List of all possible synonyms for each weapon
extern const std::vector<StringWeapon> weaponMappings;

WeaponType getWeaponTypeFromText(const std::string& buttonText);

// Weapon: active item that can perform actions (e.g., firing)
class Weapon : public Item
{
protected:
    std::unique_ptr<ValueBar> cooldown;
    std::unique_ptr<ProjectileSpawner> projSpawner;

private:
	float damage = 0.0f;    // Base damage of the weapon

public:
    Weapon(const ItemInfo&, const float&, std::unique_ptr<ProjectileSpawner>, std::unique_ptr<ValueBar>);

    virtual ~Weapon() = default;

    virtual UseResult activate(const ItemUse& use) override;

    virtual void tick(const float& deltaTime) override;

    // Render Info extension for ProjectileSpawners
    void setPosition(const sf::Vector2f& newPos) override
    {
        IMovable::setPosition(newPos);
        projSpawner->setPosition(newPos);
    }

    void addPosition(const sf::Vector2f& delta, const bool& bVelocityBased = true) override
    {
        IMovable::addPosition(delta, bVelocityBased);
        projSpawner->addPosition(delta, bVelocityBased);
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

std::unique_ptr<Weapon> makeWeapon(const std::string& weaponName);

// =============================== PERK ===============================

// Enum for various perk triggers.
enum class PerkTrigger
{
    None,                       // Fallback or uninitialized value

    OnWaveStart,                // Triggered at the beginning of a wave
    OnWaveEnd,                  // Triggered when a wave ends

    OnEnemyContact,             // When player touches or collides with enemy
    OnPlayerDamaged,            // When the player takes damage
    OnPlayerHeal,               // When the player receives healing

    OnWeaponShot,               // When a weapon is fired
    OnWeaponHit,                // When a weapon hits an enemy (successful damage application)
    OnEnemyGotHit,              // When an enemy is killed
    OnEnemyKilled,              // When an enemy is killed

    OnItemPickup,               // When picking up any item (ammo, health, power-up, etc.)

    OnInterval                  // Called at fixed intervals (e.g., every second, handled inside Perk)
};

struct PerkTriggerInfo
{
    PerkTrigger trigger;
    sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
    Entity* actor = nullptr;
};

// Perk: passive boost that triggers on specific game events.
class Perk : public Item
{
private:
    // Each perk can listen for one or more triggers.
    std::unordered_set<PerkTrigger> triggers;

public:
    Perk(const ItemInfo&, const std::unordered_set<PerkTrigger>&);
    virtual ~Perk() = default;

    virtual UseResult activate(const ItemUse& use) override;

    bool hasTrigger(const PerkTrigger& trigger);

    // Called when a game event occurs; checks if the perk should trigger.
    void tryTrigger(PerkTriggerInfo& triggerInfo);

    // For deriving classes
    virtual void onEquip()
    {
        std::cout << "OnEquip Perk" << std::endl;
    }

    virtual void onWaveStart(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnWaveStart" << std::endl;
    }

    virtual void onWaveEnd(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnWaveEnd" << std::endl;
    }

    virtual void onEnemyContact(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnEnemyContact" << std::endl;
    }

    virtual void onPlayerDamaged(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnPlayerDamaged" << std::endl;
    }

    virtual void onPlayerHeal(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnPlayerHeal" << std::endl;
    }

    virtual void onWeaponShot(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnWeaponShot" << std::endl;
    }

    virtual void onWeaponHit(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnWeaponHit" << std::endl;
    }

    virtual void onEnemyGotHit(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnEnemyGotHit" << std::endl;
    }

    virtual void onEnemyKilled(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnEnemyKill" << std::endl;
    }

    virtual void onItemPickup(PerkTriggerInfo& triggerInfo)
    {
        std::cout << "OnItemPickup" << std::endl;
    }

    virtual void onInterval(const float& deltaTime) // A perk's tick
    {
        std::cout << "On Perk Interval" << std::endl;
    }
};

std::unique_ptr<Perk> makePerk(const std::string& tag);