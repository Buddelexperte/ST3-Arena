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

    // Shield invincibility (seperate from after getting hurt)
    static constexpr bool DEFAULT_INVINCIBILITY = false;
    bool bIsInvincible = DEFAULT_INVINCIBILITY;

    // Invincible after getting hurt
    static constexpr float SPAWN_INVI = 2.0f; // 2 seconds
    static constexpr float DEFAULT_HURT_FREQ = 0.5f; // 2 seconds
    ValueBar hurtTimer;

    // Cooldown
    static constexpr float DEFAULT_COOLDOWN_MULTIPLIER = 1.0f;
    float cooldownMultiplier = DEFAULT_COOLDOWN_MULTIPLIER;
    static constexpr float DEFAULT_COOLDOWN_SUBTRACTOR = 0.0f;
    float cooldownSubtractor = DEFAULT_COOLDOWN_SUBTRACTOR;

    // Speed manipulation
    static constexpr float DEFAULT_SPEED_MULTI = 1.0f;
    float speedMulti = DEFAULT_SPEED_MULTI;

    // Health
    static constexpr float DEFAULT_HURT_BIAS = 0.0f;
    float hurtBias = DEFAULT_HURT_BIAS;
    static constexpr float DEFAULT_HURT_MULTIPLIER = 1.0f;
    float hurtMultiplier = DEFAULT_HURT_MULTIPLIER;

    // Crit
    static constexpr float DEFAUL_CRIT_CHANCE = 0.0f;
    float critChance = DEFAUL_CRIT_CHANCE;
    static constexpr float CRIT_MULITPLIER = 2.0f;

    // Pickup configs
    static constexpr float DEFAULT_MAGNETIC_DISTANCE = 200.0f;
	float magneticDistance = DEFAULT_MAGNETIC_DISTANCE; // Distance at which the player can pick up items

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

    // Shield
    void setInvincible(const bool& newVal);
    bool getIsInvincible() const;

    // Invincibility
    void setHurtFreq(const float& newVal);
    void fillHurtFreq();
    void setTimedInvincibility(const float& delay);
    void removeTimedInvincibility();
    float getHurtFreq() const;
    bool canBeHurt() const;

    // Cooldown Multiplier
    void setCooldownMultiplier(const float& newMulti);
    float getCooldownMultiplier() const;
    // Cooldown Subtractors (bias)
    void setCooldownSubtractor(const float& newSub);
    float getCooldownSubtractor() const;

    // Speed Multiplier
    void setSpeedMultiplier(const float& newVal);
    void applySpeedMultiplier(const float& newVal);
    float getSpeedMultiplier() const;

    // Set Damage values
    void setHurtBias(const float&);
    void applyHurtBias(const float&);
    float getHurtBias() const;
    void setHurtMultiplier(const float&);
    void applyHurtMultiplier(const float&);
    float getHurtMultiplier() const;

    void setCritChance(const float&);
    void addCritChance(const float&);
    float getCritChance() const;
    void applyCrit(float& damage);

    // Magnetic Range
    void setMagneticRange(const float& newRange);
    void addMagneticRange(const float& delta);
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
    void clear_inventory();

    void reset();
};