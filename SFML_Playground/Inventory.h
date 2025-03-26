#pragma once
#include "ItemBaseClasses.h"

class Player;

// Inventory class: manages all items, ensuring that only one weapon is active at a time.
class Inventory
{
private:
    Player* owner = nullptr;
    // Store all acquired weapons.
    std::string startWeaponName;
    std::vector<std::unique_ptr<Weapon>> weapons;
    int activeWeaponIndex = -1; // -1 means no active weapon

    // Store all acquired perks.
    std::vector<std::unique_ptr<Perk>> perks;

    // Multipliers
    float cooldownMultiplier = 1.0f;
    float cooldownSubtractor = 0.0f;

public:
    Inventory() = default;
    Inventory(std::unique_ptr<Weapon>);
    ~Inventory() = default;

    size_t getNumWeapons() const
        { return weapons.size(); }

    size_t getNumPerks() const
        { return perks.size(); }

    void setCooldownMultiplier(const float& newMulti)
        { cooldownMultiplier = newMulti; }

    float getCooldownMultiplier() const
        { return cooldownMultiplier; }

    void setCooldownSubtractor(const float& newSub)
        { cooldownSubtractor = newSub; }

    float getCooldownSubtractor() const
        { return cooldownSubtractor; }

    std::string getStartWeapon() const
    {
        return startWeaponName;
    }

    void setStartWeapon(const std::string& newWeaponName)
    {
        startWeaponName = newWeaponName;
    }

    // Adds a new weapon. If this is the first weapon, it becomes the active weapon.
    void addWeapon(std::unique_ptr<Weapon>);

    // Switches the active weapon by its index.
    bool selectWeapon(size_t);

    // Returns a pointer to the current active weapon.
    Weapon* getActiveWeapon() const;

    Player* getOwner()
        { return owner; }

    // Adds a new perk.
    void addPerk(std::unique_ptr<Perk>);

    // This function should be called when an in-game event occurs to trigger all relevant perks.
    void triggerPerks(PerkTrigger);

    // Removes a weapon by index and returns the unique pointer.
    std::unique_ptr<Weapon> removeWeapon(size_t);

    // Removes a perk by index and returns the unique pointer.
    std::unique_ptr<Perk> removePerk(size_t);
    
    void clear_weapons()
    {
        weapons.clear();
        activeWeaponIndex = -1;
    }

    void clear_perks()
        { perks.clear(); }

    void clear()
    {
        clear_perks();
        clear_weapons();
    }

    void reset();

    // Optional: an update function to process inventory-related logic each frame.
    void update(const float&);
};