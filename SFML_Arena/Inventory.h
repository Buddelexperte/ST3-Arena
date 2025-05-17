#pragma once

#include "ItemBaseClasses.h"
#include "LevelSystem.h"

class Player;

// Inventory class: manages all items, ensuring that only one weapon is active at a time.
class Inventory
{
private:
    Player* owner = nullptr;

    // Store all acquired weapons.
    std::string startWeaponName;
    std::vector<std::unique_ptr<Weapon>> weapons;
	int activeWeaponIndex = -1; // -1 means no active weapon or invalid index

    // Level up System
    LevelSystem levelSystem;
    unsigned int numLevelUpsQueued = 0;

    // Store all acquired perks.
    std::vector<std::unique_ptr<Perk>> perks;

    // Multipliers
    float cooldownMultiplier = 1.0f;
    float cooldownSubtractor = 0.0f;

    // Pickup configs
	float MAGNETIC_DISTANCE = 200.0f; // Distance at which the player can pick up items

    void addWeapon(std::unique_ptr<Weapon>);
    void applyPerk(std::unique_ptr<Perk>);

public:
    Inventory(Player*); // Always need to set the owning playerRef
    ~Inventory() = default;

    void tick(const float&);

    LevelSystem& getLevelSystem();

    // Queued Level Ups
    void addQueuedLevelUp();
    void removeQueuedLevelUp();
    unsigned int getNumQueuedLevelUps() const;

    // Num of items
    size_t getNumWeapons() const;
    size_t getNumPerks() const;


    // Cooldown Multiplier
    void setCooldownMultiplier(const float& newMulti);
    float getCooldownMultiplier() const;
    // Cooldown Subtractors (bias)
    void setCooldownSubtractor(const float& newSub);
    float getCooldownSubtractor() const;

    // Magnetic Range
    void setMagneticRange(const float& newRange);
    float getMagneticRange() const;

    // Start Weapon
    std::string getStartWeapon() const;
    void setStartWeapon(const std::string& newWeaponName);

    // Add Items by their string identifier
    void addWeapon_byName(const std::string& tag);
    void addPerk_byTag(const std::string& tag);

    // Removing items by index
    std::unique_ptr<Weapon> removeWeapon(size_t index);
    std::unique_ptr<Perk> removePerk(size_t index);

    // Switches the active weapon by its index. (NOT USED IN THIS GAME)
    bool selectWeapon(size_t index);

    // Returns a pointer to the current active weapon.
    Weapon* getActiveWeapon() const;
    Player* getOwner();

    // Adds a new perk.
    // This function should be called when an in-game event occurs to trigger all relevant perks.
    void triggerPerks(PerkTriggerInfo&);

    // Interact with items
    UseResult shootWeapon();
    UseResult loadUpWeapon();
    UseResult loadUpWeapon_cancel();
    
    // Clear and reset functions
    void clear_weapons();
    void clear_perks();
    void clear_all();

    void reset();
};